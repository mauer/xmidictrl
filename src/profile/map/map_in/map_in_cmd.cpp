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

#include "map_in_cmd.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_cmd::map_in_cmd(xplane &in_xp)
    : map_in(in_xp)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_cmd::type()
{
    return map_type::command;
}


/**
 * Return velocity on
 */
unsigned int map_in_cmd::velocity_on() const
{
    return m_velocity_on;
}


/**
 * Return velocity off
 */
unsigned int map_in_cmd::velocity_off() const
{
    return m_velocity_off;
}


/**
 * Read settings from config
 */
void map_in_cmd::read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'cmd'");
    map_in::read_config(in_log, in_data, in_config);

    // read the command
    m_command = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND);

    // read velocity on
    if (toml_utils::contains(in_log, in_data, CFG_KEY_VELOCITY_ON, false))
        m_velocity_on = toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_ON);

    // read velocity off
    if (toml_utils::contains(in_log, in_data, CFG_KEY_VELOCITY_OFF, false))
        m_velocity_off = toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_OFF);
}


/**
 * Check the mapping
 */
bool map_in_cmd::check(text_logger &in_log)
{
    if (!map::check(in_log))
        return false;

    if (m_command.empty()) {
        in_log.error(source_line());
        in_log.error(" --> Parameter '" + std::string(CFG_KEY_COMMAND) + "' is empty");
        return false;
    }

    if (m_velocity_on < MIDI_VELOCITY_MIN || m_velocity_on > MIDI_VELOCITY_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(CFG_KEY_VELOCITY_ON) + "', "
                     + "velocity has to be between " + std::to_string(MIDI_VELOCITY_MIN) + " and " + std::to_string(MIDI_VELOCITY_MAX));
        return false;
    }

    if (m_velocity_off < MIDI_VELOCITY_MIN || m_velocity_off > MIDI_VELOCITY_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(CFG_KEY_VELOCITY_OFF) + "', "
                     + "velocity has to be between " + std::to_string(MIDI_VELOCITY_MIN) + " and " + std::to_string(MIDI_VELOCITY_MAX));
        return false;
    }

    return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_cmd::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    in_msg.log().debug(" --> Execute command '" + m_command + "'");

    if (in_msg.data_2() == m_velocity_on) {
        xp().cmd().begin(in_msg.log(), m_command);
    } else if (in_msg.data_2() == m_velocity_off) {
        xp().cmd().end(in_msg.log(), m_command);
    } else {
        in_msg.log().error("Invalid MIDI velocity '" + std::to_string(in_msg.data_2()) + "'");
        in_msg.log().error(" --> Supported values for the current mapping are '" + std::to_string(m_velocity_on) + "' and '" +  std::to_string(m_velocity_off) + "'");
    }

    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_cmd::build_mapping_text()
{
    std::string map_str = " ====== Command ======\n";
    map_str.append("Command = '" + m_command + "'\n");

    if (m_velocity_on != MIDI_VELOCITY_MAX)
        map_str.append("Velocity on = '" + std::to_string(m_velocity_on) + "'\n");

    if (m_velocity_off != MIDI_VELOCITY_MIN)
        map_str.append("Velocity off = '" + std::to_string(m_velocity_off) + "'\n");

    return map_str;
}

} // Namespace xmidictrl