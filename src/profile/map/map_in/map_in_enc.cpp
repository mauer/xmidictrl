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

#include "map_in_enc.h"

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
map_in_enc::map_in_enc(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_enc::type()
{
    return map_type::encoder;
};


/**
 * Set the command for up
 */
void map_in_enc::set_command_up(std::string_view command_up)
{
    m_command_up = command_up;
}


/**
 * Return command for up
 */
std::string_view map_in_enc::command_up() const
{
    return m_command_up;
}


/**
 * Set the command for fast up
 */
void map_in_enc::set_command_fast_up(std::string_view command_fast_up)
{
    m_command_fast_up = command_fast_up;
}


/**
 * Return command for fast up
 */
std::string_view map_in_enc::command_fast_up() const
{
    if (m_command_fast_up.empty())
        return m_command_up;
    else
        return m_command_fast_up;
}


/**
 * Set the command for down
 */
void map_in_enc::set_command_down(std::string_view command_down)
{
    m_command_down = command_down;
}


/**
 * Return command for down
 */
std::string_view map_in_enc::command_down() const
{
    return m_command_down;
}


/**
 * Set the command for fast down
 */
void map_in_enc::set_command_fast_down(std::string_view command_fast_down)
{
    m_command_fast_down = command_fast_down;
}


/**
 * Return command for fast down
 */
std::string_view map_in_enc::command_fast_down() const
{
    if (m_command_fast_down.empty())
        return m_command_down;
    else
        return m_command_fast_down;
}


/**
 * Read settings from config
 */
void map_in_enc::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'enc'" << LOG_END
    map::read_config(settings);

    // read command up
    set_command_up(utils::read_string_parameter(settings, CFG_KEY_COMMAND_UP));

    // read command down
    set_command_down(utils::read_string_parameter(settings, CFG_KEY_COMMAND_DOWN));

    // read fast command up
    set_command_fast_up(utils::read_string_parameter(settings, CFG_KEY_COMMAND_FAST_UP, false));

    // read fast command down
    set_command_fast_down(utils::read_string_parameter(settings, CFG_KEY_COMMAND_FAST_DOWN, false));
}


/**
 * Check the mapping
 */
bool map_in_enc::check()
{
    if (!map::check())
        return false;

    if (command_up().empty() && command_down().empty() && command_fast_up().empty() && command_fast_down().empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_enc::execute(midi_message &msg)
{
    if (msg.velocity < 64) {
        // Down
        if (msg.velocity < 61) {
            LOG_DEBUG << " --> Execute command '" << command_fast_down() << "'" << LOG_END
            m_xp->cmd().execute(command_fast_down());
        } else {
            LOG_DEBUG << " --> Execute command '" << command_down() << "'" << LOG_END
            m_xp->cmd().execute(command_down());
        }
    } else if (msg.velocity > 64) {
        // Up
        if (msg.velocity > 68) {
            LOG_DEBUG << " --> Execute command '" << command_fast_up() << "'" << LOG_END
            m_xp->cmd().execute(command_fast_up());
        } else {
            LOG_DEBUG << " --> Execute command '" << command_up() << "'" << LOG_END
            m_xp->cmd().execute(command_up());
        }
    }
}

} // Namespace xmidictrl