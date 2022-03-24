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
#include "message_handler.h"

namespace xmidictrl {

class toml_utils {
public:
    toml_utils(message_handler *messages);
    ~toml_utils() = default;

    bool contains(toml::value &settings, std::string_view name, bool mandatory = true);
    bool is_array(toml::value &settings, std::string_view name);

    std::string read_string(toml::value &settings, std::string_view name, bool mandatory = true);
    std::set<std::string> read_str_set_array(toml::value &settings,
                                                    std::string_view name,
                                                    bool mandatory = true);
    std::vector<std::string> read_str_vector_array(toml::value &settings,
                                                          std::string_view name,
                                                          bool mandatory = true);
    int read_int(toml::value &settings, std::string_view name, bool mandatory = true);
    float read_float(toml::value &settings,
                            std::string_view name,
                            bool mandatory = true,
                            float fallback = 0.0f);

private:
    message_handler *m_messages;
};

} // Namespace xmidictrl

#endif // TOML_UTILS_H
