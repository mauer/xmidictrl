//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#include "conversions.h"

// Standard
#include <ctime>
#include <filesystem>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------




/**
 * Convert the outbound mapping type to a string
 */
std::string conversions::map_out_type_to_str(map_out_type in_type)
{
    switch (in_type) {
        case map_out_type::dataref:
            return "Dataref";
        case map_out_type::constant:
            return "Constant";
        case map_out_type::slider:
            return "Slider";
        case map_out_type::none:
            return {};
    }

    return {};
}


/**
 * Convert the inbound mapping type to a string
 */
std::string conversions::map_in_type_to_str(map_in_type in_type)
{
    switch (in_type) {
        case map_in_type::command:
            return "Command";
        case map_in_type::dataref:
            return "Dataref";
        case map_in_type::encoder:
            return "Encoder";
        case map_in_type::push_pull:
            return "Push&Pull";
        case map_in_type::slider:
            return "Slider";
        case map_in_type::none:
            return {};
    }

    return {};
}


/**
 * Convert an integer to a string with leading zeros
 */
std::string conversions::int_to_string(int in_number, unsigned int in_length)
{
    std::stringstream ss;
    ss << std::setw(in_length) << std::setfill('0') << in_number;
    std::string str = ss.str();

    return str;
}


} // Namespace xmidictrl
