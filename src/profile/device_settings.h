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

#ifndef XMC_DEVICE_SETTINGS_H
#define XMC_DEVICE_SETTINGS_H

// Standard
#include <set>
#include <string>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

struct device_settings {
    std::string name;

    int device_no {-1};

    int port_in {-1};
    int port_out {-1};

    std::set<std::string> include;

    mode_note mode_note {mode_note::on_off};
    mode_out mode_out {mode_out::permanent};

    float outbound_delay {0.5f};

    encoder_mode default_enc_mode {encoder_mode::relative};
};

} // Namespace xmidictrl

#endif // XMC_DEVICE_SETTINGS_H