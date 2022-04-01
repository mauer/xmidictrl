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

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in::map_in(xplane *in_xp)
    : map(in_xp)
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
void map_in::read_config(text_logger *in_log, toml::value &in_data)
{
    map::read_config(in_log, in_data);

    // additional config
    read_sublayer(in_log, in_data);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter sl
 */
void map_in::read_sublayer(text_logger *in_log, toml::value &in_data)
{
    m_sl.clear();

    try {
        // read sublayer
        if (in_data.contains(CFG_KEY_SL)) {
            m_sl = in_data[CFG_KEY_SL].as_string();

            in_log->debug(" --> Line %i :: Parameter '%s' = '%s'", in_data.location().line(), CFG_KEY_SL, m_sl.c_str());
        }
    } catch (toml::type_error &error) {
        in_log->error(" --> Line %i :: Error reading mapping", in_data.location().line());
        in_log->error(error.what());
    }
}


/**
 * Check if the command is defined for the current sublayer
 */
bool map_in::check_sublayer(std::string_view in_sl_value)
{
    if (in_sl_value != m_sl && !m_sl.empty())
        return false;

    return true;
}

} // Namespace xmidictrl