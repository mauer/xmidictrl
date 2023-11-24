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
 * Return the dataref mode for a given string
 */
dataref_mode conversions::dataref_mode_from_code(std::string_view in_mode)
{
    if (in_mode == "momentary")
        return dataref_mode::momentary;
    else
        return dataref_mode::toggle;
}


/**
 * Return the encoder mode for a given string
 */
encoder_mode conversions::encoder_mode_from_code(std::string_view in_mode)
{
    if (in_mode == "range")
        return encoder_mode::range;
    else
        return encoder_mode::relative;
}


/**
 * Convert a time point into a string
 */
std::string conversions::time_to_string(time_point in_time)
{
    std::time_t t = std::chrono::system_clock::to_time_t(in_time);
    std::tm time_info = *std::localtime(&t);

    // format into a string
    std::ostringstream oss;
    oss << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S");

    return oss.str();
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


/**
 * Convert an float to a string with a number of decimals
 */
std::string conversions::float_to_string(float in_number, unsigned int in_precision)
{
    std::stringstream ss;
    ss << std::setprecision(in_precision) << in_number;
    std::string str = ss.str();

    return str;
}


/**
 * Create a unique key for mappings
 */
std::string conversions::create_map_key(const unsigned char in_ch, std::string_view in_type_code, const unsigned char in_data)
{
    std::stringstream ss;
    ss << static_cast<int>(in_ch) << in_type_code << static_cast<int>(in_data);

    return ss.str();
}

} // Namespace xmidictrl
