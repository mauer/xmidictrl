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
map_in_pnp::map_in_pnp(xplane *in_xp)
    : map_in(in_xp)
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
};


/**
 * Set the push command
 */
void map_in_pnp::set_command_push(std::string_view in_command)
{
    m_command_push = in_command;
}


/**
 * Return the push command
 */
std::string_view map_in_pnp::command_push() const
{
    return m_command_push;
}


/**
 * Set the pull command
 */
void map_in_pnp::set_command_pull(std::string_view in_command)
{
    m_command_pull = in_command;
}


/**
 * Return the pull command
 */
std::string_view map_in_pnp::command_pull() const
{
    return m_command_pull;
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
void map_in_pnp::read_config(text_logger *in_log, toml::value &in_data)
{
    in_log->debug(" --> Line %i :: Read settings for type 'pnp'", in_data.location().line());
    map_in::read_config(in_log, in_data);

    // read command push
    set_command_push(toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_PUSH));

    // read command pull
    set_command_pull(toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_PULL));
}


/**
 * Check the mapping
 */
bool map_in_pnp::check(text_logger *in_log)
{
    if (!map::check(in_log))
        return false;

    if (m_command_push.empty() && m_command_pull.empty())
        return false;
    else
        return true;
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
    if (m_command_type != command_type::none && m_time_command > time_point::min()) {
        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - m_time_command;

        if (elapsed.count() > 0.3f) {
            switch (m_command_type) {
                case command_type::push:
                    in_msg.log()->debug(" --> End push command '%s'", m_command_push.c_str());
                    m_xp->cmd().end(in_msg.log(), m_command_push);
                    break;

                case command_type::pull:
                    in_msg.log()->debug(" --> End pull command '%s'", m_command_pull.c_str());
                    m_xp->cmd().end(in_msg.log(), m_command_pull);
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
        // failsafe, don't want to keep this task in the lst forever
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - m_time_received.load();

        if (elapsed_seconds.count() < 0.5f)
            return false;
    }

    std::chrono::duration<double> elapsed_seconds = m_time_released.load() - m_time_received.load();
    m_time_command = std::chrono::system_clock::now();

    if (elapsed_seconds.count() > 0.5f || m_time_released.load() == time_point::min()) {
        in_msg.log()->debug(" --> Begin pull command '%s'", m_command_pull.c_str());
        m_command_type = command_type::pull;
        m_xp->cmd().begin(in_msg.log(), m_command_pull);
    } else {
        in_msg.log()->debug(" --> Begin push command '%s'", m_command_push.c_str());
        m_command_type = command_type::push;
        m_xp->cmd().begin(in_msg.log(), m_command_push);
    }

    // keep the task in the queue to end the command
    return false;
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