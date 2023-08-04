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

#ifndef XMC_CONVERSIONS_H
#define XMC_CONVERSIONS_H

// Standard
#include <set>
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "environment.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class conversions {
public:
    static mode_out mode_out_from_int(int in_mode);

    // move directly into mapping classes
    static dataref_mode dataref_mode_from_code(std::string_view in_mode);
    static encoder_mode encoder_mode_from_code(std::string_view in_mode);

    static std::string time_to_string(time_point in_time);
    static std::string int_to_string(int in_number, unsigned int in_length);

    static std::string create_map_key(unsigned char in_ch, std::string_view in_type_code, unsigned char in_data);
};

} // Namespace xmidictrl

#endif // XMC_CONVERSIONS_H
