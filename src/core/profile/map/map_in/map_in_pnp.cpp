//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
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
map_in_pnp::map_in_pnp(app_services &in_app)
    : map_in(in_app)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_pnp::type()
{
    return map_type::push_pull;
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
void map_in_pnp::read_config(text_logger &in_log, toml::value &in_data, device &in_device, toml::value &in_config)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'pnp'");
    map_in::read_config(in_log, in_data, in_device, in_config);

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

    if (!mapping::check(in_log))
        result = false;

    // pull
    if (!m_dataref_pull.empty()) {
        if (!app().datarefs().check(m_dataref_pull)) {
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
        if (!app().datarefs().check(m_dataref_push)) {
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
                        app().cmd().end(in_msg.log(), m_command_push);
                    }
                    break;

                case command_type::pull:
                    if (!m_command_pull.empty()) {
                        in_msg.log().debug(" --> End pull command '" + m_command_pull + "'");
                        app().cmd().end(in_msg.log(), m_command_pull);
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
            app().cmd().begin(in_msg.log(), m_command_pull);

            // keep the task in the queue to end the command
            return false;
        }
    } else {
        if (!m_dataref_push.empty()) {
            toggle_dataref(in_msg.log(), m_dataref_push, m_values_push);
        } else if (!m_command_push.empty()) {
            in_msg.log().debug(" --> Begin push command '" + m_command_push + "'");
            m_command_type = command_type::push;
            app().cmd().begin(in_msg.log(), m_command_push);

            // keep the task in the queue to end the command
            return false;
        }
    }

    // don't keep the task in the queue
    return true;
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




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_pnp::build_mapping_text()
{
    std::string map_str = " ====== Push & Pull ======\n";
    map_str.append("Command push = '" + m_command_push + "'\n");
    map_str.append("Command pull = '" + m_command_pull + "'\n");

    return map_str;
}

} // Namespace xmidictrl