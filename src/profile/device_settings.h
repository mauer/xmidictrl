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
#include <string_view>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

// Outbound note mode (on off vs on)
enum class outbound_note_mode {
    on_off,
    on
};

// Outbound send mode
enum class outbound_send_mode {
    on_change,
    permanent
};

// All possible device settings
struct device_settings {
    std::string name;

    int device_no {-1};

    int port_in {-1};
    int port_out {-1};

    std::set<std::string> include {};

    outbound_note_mode note_mode {outbound_note_mode::on_off};
    outbound_send_mode send_mode {outbound_send_mode::permanent};

    float outbound_delay {0.5f};

    std::string sl_dataref {};

    encoder_mode default_enc_mode {encoder_mode::relative};

    // Return the outbound note mode for a given code
    static outbound_note_mode note_mode_from_code(std::string_view in_mode)
    {
        if (in_mode == "on")
            return outbound_note_mode::on;
        else
            return outbound_note_mode::on_off;
    }

     // Return the outbound mode for a given code
    static outbound_send_mode send_mode_from_code(std::string_view in_mode)
    {
        if (in_mode == "on_change")
            return outbound_send_mode::on_change;
        else
            return outbound_send_mode::permanent;
    }
};

} // Namespace xmidictrl

#endif // XMC_DEVICE_SETTINGS_H