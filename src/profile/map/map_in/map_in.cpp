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

#include "map_in.h"

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in::map_in(xplane *xp)
    : map(xp)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the sublayer name
 */
std::string_view map_in::sl() const
{
    return m_sl;
}


/**
 * Read the config
 */
void map_in::read_config(message_handler *messages, toml::value &data)
{
    map::read_config(messages, data);

    // additional config
    read_sublayer(data);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter sl
 */
void map_in::read_sublayer(toml::value &data)
{
    m_sl.clear();

    try {
        // read sublayer
        if (data.contains(CFG_KEY_SL)) {
            m_sl = data[CFG_KEY_SL].as_string();

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_SL << "' = '" << m_sl
                      << "'" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << " --> Line " << data.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}


/**
 * Check if the command is defined for the current sublayer
 */
bool map_in::check_sublayer(std::string_view sl_value)
{
    if (sl_value != m_sl && !m_sl.empty())
        return false;

    return true;
}

} // Namespace xmidictrl