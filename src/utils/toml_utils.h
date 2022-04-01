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

#ifndef TOML_UTILS_H
#define TOML_UTILS_H

// Standard
#include <set>
#include <string>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "text_logger.h"

namespace xmidictrl {

class toml_utils {
public:
    static bool contains(text_logger *in_log,
                         toml::value &in_data,
                         std::string_view in_name,
                         bool in_mandatory = true);
    static bool is_array(text_logger *in_log, toml::value &in_data, std::string_view in_name);

    static std::string read_string(text_logger *in_log,
                                   toml::value &in_data,
                                   std::string_view in_name,
                                   bool in_mandatory = true);

    static std::set<std::string> read_str_set_array(text_logger *in_log, toml::value &in_data,
                                                    std::string_view in_name,
                                                    bool in_mandatory = true);

    static std::vector<std::string> read_str_vector_array(text_logger *in_log, toml::value &in_data,
                                                          std::string_view in_name,
                                                          bool in_mandatory = true);

    static int read_int(text_logger *in_log,
                        toml::value &in_data,
                        std::string_view in_name,
                        bool in_mandatory = true);

    static float read_float(text_logger *in_log, toml::value &in_data,
                            std::string_view in_name,
                            bool in_mandatory = true,
                            float in_fallback = 0.0f);
};

} // Namespace xmidictrl

#endif // TOML_UTILS_H
