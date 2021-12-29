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
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class utils {
public:
    static bool toml_contains(toml::value &settings, std::string_view name, bool mandatory = true);

    static std::string toml_read_string(toml::value &settings, std::string_view name, bool mandatory = true);
    static std::vector<std::string> toml_read_string_array(toml::value &settings,
                                                           std::string_view name,
                                                           bool mandatory = true);
    static int toml_read_int(toml::value &settings, std::string_view name, bool mandatory = true);

    static std::string text_msg_type_as_text(text_msg_type type);
    static std::string midi_msg_type_as_text(midi_type type);

    static std::string log_level_as_text(log_level level);
    static std::string log_level_as_code(log_level level);
    static log_level log_level_from_code(std::string_view code);

    static mode_out mode_out_from_int(int mode);

    static std::string time_to_string(time_point time);
    static std::string ch_cc(unsigned int ch, unsigned int cc);

    static bool create_preference_folders(const std::shared_ptr<xplane> &xp);
};

} // Namespace xmidictrl

#endif // UTILS_H
