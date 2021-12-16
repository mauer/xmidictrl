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
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_pnp::map_in_pnp(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
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
void map_in_pnp::set_command_push(std::string_view command)
{
    m_command_push = command;
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
void map_in_pnp::set_command_pull(std::string_view command)
{
    m_command_pull = command;
}


/**
 * Return the pull command
 */
std::string_view map_in_pnp::command_pull() const
{
    return m_command_pull;
}


/**
 * Set the command type
 */
void map_in_pnp::set_command_type(CommandType commandType)
{
    m_command_type = commandType;
}


/**
 * Read settings from config
 */
void map_in_pnp::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'pnp'" << LOG_END
    map::read_config(settings);

    // read command push
    set_command_push(utils::read_string_parameter(settings, CFG_KEY_COMMAND_PUSH));

    // read command pull
    set_command_pull(utils::read_string_parameter(settings, CFG_KEY_COMMAND_PULL));
}


/**
 * Check the mapping
 */
bool map_in_pnp::check()
{
    if (!map::check())
        return false;

    if (m_command_push.empty() && m_command_pull.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_pnp::execute(midi_message &mmsg)
{
    switch (m_command_type) {
        case CommandType::Push:
            LOG_DEBUG << " --> Execute command '" << m_command_push << "'" << LOG_END
            m_xp->cmd().execute(m_command_push);
            break;

        case CommandType::Pull:
            LOG_DEBUG << " --> Execute command '" << m_command_pull << "'" << LOG_END
            m_xp->cmd().execute(m_command_pull);
            break;
    }
}

} // Namespace xmidictrl