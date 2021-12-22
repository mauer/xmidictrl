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

#include "map_in_sld.h"

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
map_in_sld::map_in_sld(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_sld::type()
{
    return map_type::slider;
};


/**
 * Set the up command
 */
void map_in_sld::set_command_up(std::string_view command)
{
    m_command_up = command;
}


/**
 * Return the up command
 */
std::string_view map_in_sld::command_up() const
{
    return m_command_up;
}


/**
 * Set the middle command
 */
void map_in_sld::set_command_middle(std::string_view command)
{
    m_command_middle = command;
}


/**
 * Return the middle command
 */
std::string_view map_in_sld::command_middle() const
{
    return m_command_middle;
}


/**
 * Set the down command
 */
void map_in_sld::set_command_down(std::string_view command)
{
    m_command_down = command;
}


/**
 * Return the down command
 */
std::string_view map_in_sld::command_down() const
{
    return m_command_down;
}


/**
 * Read settings from config
 */
void map_in_sld::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'sld'" << LOG_END
    map::read_config(settings);

    // read command up
    set_command_up(utils::toml_read_string(settings, CFG_KEY_COMMAND_UP));

    // read command middle
    set_command_middle(utils::toml_read_string(settings, CFG_KEY_COMMAND_MIDDLE));

    // read command down
    set_command_down(utils::toml_read_string(settings, CFG_KEY_COMMAND_DOWN));
}


/**
 * Check the mapping
 */
bool map_in_sld::check()
{
    if (!map::check())
        return false;

    if (m_command_up.empty() && m_command_down.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_sld::execute(midi_message &msg, std::string_view sl_value)
{
    if (!check_sublayer(sl_value))
        return true;

    if (msg.velocity <= 10) {
        LOG_DEBUG << " --> Execute command '" << m_command_down << "'" << LOG_END
        m_xp->cmd().execute(m_command_down);
    } else if (msg.velocity >= 117) {
        LOG_DEBUG << " --> Execute command '" << m_command_up << "'" << LOG_END
        m_xp->cmd().execute(m_command_up);
    } else if (msg.velocity >= 50 && msg.velocity <= 70) {
        if (!m_command_middle.empty()) {
            LOG_DEBUG << " --> Execute command '" << m_command_middle << "'" << LOG_END
            m_xp->cmd().execute(m_command_middle);
        }
    }

    return true;
}

} // Namespace xmidictrl