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
 * Read settings from config
 */
void map_in_cmd::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'cmd'" << LOG_END
    map_in::read_config(settings);

    // read the command
    set_command(utils::toml_read_string(settings, CFG_KEY_COMMAND));
}


/**
 * Check the mapping
 */
bool map_in_cmd::check()
{
    if (!map::check())
        return false;

    if (m_command.empty())
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

    LOG_DEBUG << " --> Execute command '" << m_command << "'" << LOG_END

    switch (msg.velocity) {
        case 127:
            m_xp->cmd().begin(m_command);
            break;

        case 0:
            m_xp->cmd().end(m_command);
            break;

        default:
            LOG_ERROR << "Invalid midi velocity '" << msg.velocity << "'" << LOG_END
            LOG_ERROR << " --> Supported values are '127' and '0'" << LOG_END
    }

    return true;
}

} // Namespace xmidictrl