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
#include "map_in.h"
#include "map_out.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class conversions {
public:
    static std::string map_out_type_to_str(map_out_type in_type);
    static std::string map_in_type_to_str(map_in_type in_type);

    static std::string int_to_string(int in_number, unsigned int in_length);
};

} // Namespace xmidictrl

#endif // XMC_CONVERSIONS_H
