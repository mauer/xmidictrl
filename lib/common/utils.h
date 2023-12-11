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

#ifndef XMC_UTILS_H
#define XMC_UTILS_H

// Standard
#include <regex>
#include <string>

// XMidiCtrl
#include "text_logger.h"

namespace xmidictrl {

class utils {
public:
    static bool create_directory(text_logger& in_log, const std::filesystem::path& in_path);

    // TODO: Move to cpp file
    static std::string ltrim(std::string_view in_str) {
        return std::regex_replace(in_str.data(), std::regex("^\\s+"), std::string());
    }


    static std::string rtrim(std::string_view in_str) {
        return std::regex_replace(in_str.data(), std::regex("\\s+$"), std::string());
    }


    static std::string trim(std::string_view in_str) {
        return ltrim(rtrim(in_str.data()));
    }

    static std::string time_to_string(time_point in_time);

    static std::string float_to_string(float in_number, unsigned int in_precision = 2);
};

} // Namespace xmidictrl

#endif // XMC_UTILS_H
