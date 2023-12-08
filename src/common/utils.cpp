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

#include "utils.h"

// Standard
#include <filesystem>
#include <time.h>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create a directory
 */
bool utils::create_directory(text_logger& in_log, const std::filesystem::path& in_path)
{
    // check includes folder
    if (!std::filesystem::exists(in_path)) {
        in_log.info("Directory '" + in_path.string() + "' not found");

        if (std::filesystem::create_directory(in_path)) {
            in_log.info("Directory '" + in_path.string() + "' created");
        } else {
            in_log.error("Could not create directory '" + in_path.string() + "'");
            return false;
        }
    }

    return true;
}


/**
 * Convert a time point into a string
 */
std::string utils::time_to_string(time_point in_time)
{
    std::time_t t = std::chrono::system_clock::to_time_t(in_time);

#ifdef _MSC_VER
    std::tm time_info {};
    localtime_s(&time_info, &t);
#else
    std::tm time_info = *std::localtime(&t);
#endif

    // format into a string
    std::ostringstream oss;
    oss << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}


/**
 * Convert an float to a string with a number of decimals
 */
std::string utils::float_to_string(float in_number, unsigned int in_precision)
{
    std::stringstream ss;
    ss << std::setprecision(in_precision) << in_number;
    std::string str = ss.str();

    return str;
}

} // Namespace xmidictrl
