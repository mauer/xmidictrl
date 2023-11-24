//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#include "map_in_pnp.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_pnp::map_in_pnp(environment &in_env)
    : map_in(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_pnp::type()
{
    return map_in_type::push_pull;
}


/**
 * Return the mapping type as string
 */
std::string map_in_pnp::type_as_string()
{
    return "Push&Pull";
}


/**
 * Set the time point when the initial message was received
 */
void map_in_pnp::set_time_received()
{
    // reset all times first
    reset();

    m_time_received.store(std::chrono::system_clock::now());
}


/**
 * Set the time point when the initial message was released
 */
void map_in_pnp::set_time_released()
{
    // only set the released time, if a received time is present
    if (m_time_received.load() > time_point::min())
        m_time_released.store(std::chrono::system_clock::now());
}


/**
 * Read settings from config
 */
void map_in_pnp::read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'pnp'");
    map_in::read_config(in_log, in_data, in_config);

    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF_PUSH, false)) {
        // read dataref push
        m_dataref_push = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF_PUSH, false);
        m_values_push = toml_utils::read_str_vector_array(in_log, in_data, CFG_KEY_VALUES_PUSH, false);
    } else {
        // read command push
        m_command_push = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_PUSH, false);
    }

    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF_PULL, false)) {
        // read dataref pull
        m_dataref_pull = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF_PULL, false);
        m_values_pull = toml_utils::read_str_vector_array(in_log, in_data, CFG_KEY_VALUES_PULL, false);
    } else {
        // read command pull
        m_command_pull = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_PULL, false);
    }
}


/**
 * Check the mapping
 */
bool map_in_pnp::check(text_logger &in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    // pull
    if (!m_dataref_pull.empty()) {
        if (!env().drf().check(m_dataref_pull)) {
            in_log.error(source_line());
            in_log.error(" --> Dataref '" + std::string(m_dataref_pull) + "' not found");
            result = false;
        }

        if (m_values_pull.empty()) {
            in_log.error(source_line());
            in_log.error(" --> Parameter '" + std::string(CFG_KEY_VALUES_PULL) + "' is not defined");
            result = false;
        }
    } else {
        if (m_command_pull.empty()) {
            in_log.error(source_line());
            in_log.error(" --> Parameter '" + std::string(CFG_KEY_COMMAND_PULL) + "' is empty");
            result = false;
        }
    }

    // push
    if (!m_dataref_push.empty()) {
        if (!env().drf().check(m_dataref_push)) {
            in_log.error(source_line());
            in_log.error(" --> Dataref '" + std::string(m_dataref_push) + "' not found");
            result = false;
        }

        if (m_values_push.empty()) {
            in_log.error(source_line());
            in_log.error(" --> Parameter '" + std::string(CFG_KEY_VALUES_PUSH) + "' is not defined");
            result = false;
        }
    } else {
        if (m_command_push.empty()) {
            in_log.error(source_line());
            in_log.error(" --> Parameter '" + std::string(CFG_KEY_COMMAND_PUSH) + "' is empty");
            result = false;
        }
    }

    return result;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_pnp::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value) || m_time_received.load() == time_point::min()) {
        // wrong sublayer (or received time is missing)
        reset();
        return true;
    }

    // check if the command has been already executed
    if (m_time_command > time_point::min()) {
        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - m_time_command;

        if (elapsed.count() > 0.3f) {
            switch (m_command_type) {
                case command_type::push:
                    if (!m_command_push.empty()) {
                        in_msg.log().debug(" --> End push command '" + m_command_push + "'");
                        env().cmd().end(in_msg.log(), m_command_push);
                    }
                    break;

                case command_type::pull:
                    if (!m_command_pull.empty()) {
                        in_msg.log().debug(" --> End pull command '" + m_command_pull + "'");
                        env().cmd().end(in_msg.log(), m_command_pull);
                    }
                    break;

                case command_type::none:
                    break;
            }

            reset();
            return true;
        } else {
            // let's wait a bit longer...
            return false;
        }
    }

    if (m_time_released.load() == time_point::min()) {
        // failsafe, don't want to keep this task in the list forever
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - m_time_received.load();

        if (elapsed_seconds.count() < 0.5f)
            return false;
    }

    std::chrono::duration<double> elapsed_seconds = m_time_released.load() - m_time_received.load();
    m_time_command = std::chrono::system_clock::now();

    if (elapsed_seconds.count() > 0.5f || m_time_released.load() == time_point::min()) {
        if (!m_dataref_pull.empty()) {
            toggle_dataref(in_msg.log(), m_dataref_pull, m_values_pull);
        } else if (!m_command_pull.empty()) {
            in_msg.log().debug(" --> Begin pull command '" + m_command_pull + "'");
            m_command_type = command_type::pull;
            env().cmd().begin(in_msg.log(), m_command_pull);

            // keep the task in the queue to end the command
            return false;
        }
    } else {
        if (!m_dataref_push.empty()) {
            toggle_dataref(in_msg.log(), m_dataref_push, m_values_push);
        } else if (!m_command_push.empty()) {
            in_msg.log().debug(" --> Begin push command '" + m_command_push + "'");
            m_command_type = command_type::push;
            env().cmd().begin(in_msg.log(), m_command_push);

            // keep the task in the queue to end the command
            return false;
        }
    }

    // don't keep the task in the queue
    return true;
}


/**
 * Return mapped command
 */
std::string map_in_pnp::map_text_cmd_drf()
{
    std::string map_str {};

    if (!m_dataref_push.empty())
        map_str.append(m_dataref_push + "   (dataref push)");
    else
        map_str.append(m_command_push + "   (command push)");

    if (!m_dataref_pull.empty())
        map_str.append("\n" + m_dataref_pull + "   (dataref pull)");
    else
        map_str.append("\n" + m_command_pull + "   (command pull)");

    return map_str;
}


/**
 * Return mapped parameter
 */
std::string map_in_pnp::map_text_parameter()
{
    std::string map_str {};

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_pnp::build_mapping_text(bool in_short)
{
    std::string map_str;

    if (!in_short) {
        map_str = " ====== Push & Pull ======\n";

        if (!sl().empty())
            map_str.append("Sublayer = '" + std::string(sl()) + "'\n");
    }

    // TODO: Add dataref option

    map_str.append("Cmd Push: " + m_command_push + "\n");
    map_str.append("Cmd Pull: " + m_command_pull);

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Reset all data
 */
void map_in_pnp::reset()
{
    m_command_type = command_type::none;
    m_time_command = time_point::min();

    m_time_received.store(time_point::min());
    m_time_released.store(time_point::min());
}

} // Namespace xmidictrl