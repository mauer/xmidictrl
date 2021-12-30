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
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_cmd::map_in_cmd(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
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
};


/**
 * Set the command
 */
void map_in_cmd::set_command(std::string_view command)
{
    m_command = command;
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
void map_in_cmd::set_velocity_on(int velocity_on)
{
    if (velocity_on >= 0 && velocity_on <= 127)
        m_velocity_on = velocity_on;
    else
        m_velocity_on = 127;
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
void map_in_cmd::set_velocity_off(int velocity_off)
{
    if (velocity_off >= 0 && velocity_off <= 127)
        m_velocity_off = velocity_off;
    else
        m_velocity_off = 0;
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
void map_in_cmd::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'cmd'" << LOG_END
    map_in::read_config(settings);

    // read the command
    set_command(utils::toml_read_string(settings, CFG_KEY_COMMAND));

    // read velocity on
    set_velocity_on(utils::toml_read_int(settings, CFG_KEY_VELOCITY_ON, false));

    // read velocity off
    set_velocity_off(utils::toml_read_int(settings, CFG_KEY_VELOCITY_OFF, false));
}


/**
 * Check the mapping
 */
bool map_in_cmd::check()
{
    if (!map::check())
        return false;

    if (command().empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_cmd::execute(midi_message &msg, std::string_view sl_value)
{
    if (!check_sublayer(sl_value))
        return true;

    LOG_DEBUG << " --> Execute command '" << command() << "'" << LOG_END

    if (msg.velocity == velocity_on()) {
        m_xp->cmd().begin(command());
    } else if (msg.velocity == velocity_off()) {
        m_xp->cmd().end(command());
    } else {
        LOG_ERROR << "Invalid MIDI velocity '" << msg.velocity << "'" << LOG_END
        LOG_ERROR << " --> Supported values for the current mapping are '" << velocity_on() << "' and '"
                  << velocity_off() << "'" << LOG_END
    }

    return true;
}

} // Namespace xmidictrl