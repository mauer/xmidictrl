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

#include <utility>

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
 * Return the mapping as string
 */
std::string map_in_cmd::as_string()
{
    std::string map_str = " :: Command ::\n";
    map_str.append("Command = '" + m_command + "'\n");

    if (m_velocity_on != MIDI_VELOCITY_MAX)
        map_str.append("Velocity on = '" + std::to_string(m_velocity_on) + "'\n");

    if (m_velocity_off != MIDI_VELOCITY_MIN)
        map_str.append("Velocity off = '" + std::to_string(m_velocity_off) + "'\n");

    return map_str;
}


/**
 * Set the command
 */
void map_in_cmd::set_command(std::string_view in_command)
{
    m_command = in_command;
}


/**
 * Return the command
 */
std::string_view map_in_cmd::command() const
{
    return m_command;
}


/**
 * Set velocity on
 */
void map_in_cmd::set_velocity_on(int in_velocity_on)
{
    if (in_velocity_on >= MIDI_VELOCITY_MIN && in_velocity_on <= MIDI_VELOCITY_MAX)
        m_velocity_on = in_velocity_on;
    else
        m_velocity_on = MIDI_VELOCITY_MAX;
}


/**
 * Return velocity on
 */
unsigned int map_in_cmd::velocity_on() const
{
    return m_velocity_on;
}


/**
 * Set velocity off
 */
void map_in_cmd::set_velocity_off(int in_velocity_off)
{
    if (in_velocity_off >= MIDI_VELOCITY_MIN && in_velocity_off <= MIDI_VELOCITY_MAX)
        m_velocity_off = in_velocity_off;
    else
        m_velocity_off = MIDI_VELOCITY_MIN;
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
void map_in_cmd::read_config(text_logger &in_log, toml::value &in_data)
{
    in_log.debug(" --> Line %i :: Read settings for type 'cmd'", in_data.location().line());
    map_in::read_config(in_log, in_data);

    // read the command
    set_command(toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND));

    // read velocity on
    set_velocity_on(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_ON, false));

    // read velocity off
    set_velocity_off(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_OFF, false));
}


/**
 * Check the mapping
 */
bool map_in_cmd::check(text_logger &in_log)
{
    if (!map::check(in_log))
        return false;

    if (command().empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_cmd::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    in_msg.log().debug(" --> Execute command '%s'", command().data());

    if (in_msg.data_2() == velocity_on()) {
        xp().cmd().begin(in_msg.log(), command());
    } else if (in_msg.data_2() == velocity_off()) {
        xp().cmd().end(in_msg.log(), command());
    } else {
        in_msg.log().error("Invalid MIDI velocity '%i'", in_msg.data_2());
        in_msg.log().error(" --> Supported values for the current mapping are '%i' and '%io'", velocity_on(), velocity_off());
    }

    return true;
}

} // Namespace xmidictrl