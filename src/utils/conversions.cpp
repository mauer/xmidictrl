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

#include "conversions.h"

// Standard
#include <ctime>
#include <filesystem>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the outbound mode for a given integer
 */
mode_out conversions::mode_out_from_int(int mode)
{
    if (mode == 1)
        return mode_out::on_change;
    else
        return mode_out::permanent;
}


/**
 * Return the dataref mode for a given string
 */
dataref_mode conversions::dataref_mode_from_code(std::string_view mode)
{
    if (mode == "momentary")
        return dataref_mode::momentary;
    else
        return dataref_mode::toggle;
}


/**
 * Return the encoder mode for a given string
 */
encoder_mode conversions::encoder_mode_from_code(std::string_view mode)
{
    if (mode == "range")
        return encoder_mode::range;
    else
        return encoder_mode::relative;
}


/**
 * Convert a time point into a string
 */
std::string conversions::time_to_string(time_point time)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&t);

    // format into a string
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}


/**
 * Create a unique key for mappings
 */
std::string conversions::create_map_key(const unsigned char ch, std::string_view type_code, const unsigned char data)
{
    std::stringstream ss;
    ss << static_cast<int>(ch) << type_code << static_cast<int>(data);

    return ss.str();
}


/**
 * Create all required preference folders
 */
bool conversions::create_preference_folders(text_logger &in_log, xplane &in_xp)
{
    // check preference folder
    if (!std::filesystem::exists(in_xp.preferences_path())) {
        in_log.info("Directory '%s' not found", in_xp.preferences_path().data());

        if (std::filesystem::create_directory(in_xp.preferences_path())) {
            in_log.info("Directory '%s' created", in_xp.preferences_path().data());
        } else {
            in_log.error("Could not create directory '%s'", in_xp.preferences_path().data());
            return false;
        }
    }

    // check profiles folder
    if (!std::filesystem::exists(in_xp.profiles_path())) {
        in_log.info("Directory '%s' not found", in_xp.profiles_path().data());

        if (std::filesystem::create_directory(in_xp.profiles_path())) {
            in_log.info("Directory '%s' created", in_xp.profiles_path().data());
        } else {
            in_log.error("Could not create directory '%s'", in_xp.profiles_path().data());
            return false;
        }
    }

    return true;
}

} // Namespace xmidictrl
