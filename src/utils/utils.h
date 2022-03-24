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

#ifndef UTILS_H
#define UTILS_H

// Standard
#include <set>
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "message_handler.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class utils {
public:
    //static bool toml_contains(toml::value &settings, std::string_view name, bool mandatory = true);
    //static bool toml_is_array(toml::value &settings, std::string_view name);

    //static std::string toml_read_string(toml::value &settings, std::string_view name, bool mandatory = true);
    //static std::set<std::string> toml_read_str_set_array(toml::value &settings,
    //                                                     std::string_view name,
    //                                                     bool mandatory = true);
    //static std::vector<std::string> toml_read_str_vector_array(toml::value &settings,
    //                                                       std::string_view name,
    //                                                       bool mandatory = true);
    //static int toml_read_int(toml::value &settings, std::string_view name, bool mandatory = true);
    //static float toml_read_float(toml::value &settings,
    //                             std::string_view name,
    //                             bool mandatory = true,
    //                             float fallback = 0.0f);

    static std::string log_level_as_text(log_level level);
    static std::string log_level_as_code(log_level level);
    static log_level log_level_from_code(std::string_view code);

    static mode_out mode_out_from_int(int mode);

    // move directly into mapping classes
    static dataref_mode dataref_mode_from_code(std::string_view mode);
    static encoder_mode encoder_mode_from_code(std::string_view mode);

    static std::string time_to_string(time_point time);

    static std::string create_map_key(const unsigned char ch, std::string_view type_code, const unsigned char data);

    static bool create_preference_folders(xplane *xp);
};

} // Namespace xmidictrl

#endif // UTILS_H
