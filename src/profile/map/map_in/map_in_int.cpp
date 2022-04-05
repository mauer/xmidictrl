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

#include "map_in_int.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_int::map_in_int(xplane &in_xp, std::string_view in_command)
    : map_in(in_xp),
      m_command(in_command)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_int::type()
{
    return map_type::internal;
}


/**
 * Check the mapping
 */
bool map_in_int::check(text_logger &in_log)
{
    if (!map::check(in_log))
        return false;

    if (m_command.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_int::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------


/**
 * Return the mapping as string
 */
std::string map_in_int::build_mapping_text()
{
    std::string map_str = " :: Internal ::\n";

    return map_str;
}

} // Namespace xmidictrl