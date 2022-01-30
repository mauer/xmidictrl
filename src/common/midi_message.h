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

#ifndef MIDI_MESSAGE_H
#define MIDI_MESSAGE_H

// Standard
#include <chrono>

// XMidiCtrl
#include "types.h"
#include "utils.h"

namespace xmidictrl {

struct midi_message {
    time_point time {time_point::min()};
    msg_direction direction {msg_direction::inbound};

    unsigned int port {0};

    unsigned char status {MIDI_NONE};
    unsigned char data {MIDI_NONE};
    unsigned char velocity {MIDI_NONE};

    // Return the direction as text
    [[nodiscard]] std::string direction_text() const
    {
        switch (direction) {
            case msg_direction::inbound:
                return "Inbound";

            case msg_direction::outbound:
                return "Outbound";

            default:
                return "<unknown>";
        }
    }

    // Get the channel
    [[nodiscard]] unsigned char get_channel() const
    {
        if ((status & 0xf0) != 0xf0)
            return (status & 0xf) + 1;
        else
            return MIDI_NONE;
    }

    // Get MIDI message type
    [[nodiscard]] midi_msg_type get_type() const
    {
        switch (status & 0xf0) {
            case 0xb0:
                return midi_msg_type::control_change;

            case 0x90:
                return midi_msg_type::note_on;

            case 0x80:
                return midi_msg_type::note_off;

            case 0xc0:
                return midi_msg_type::program_change;

            case 0xa0:
                return midi_msg_type::aftertouch;

            case 0xd0:
                return midi_msg_type::channel_pressure;

            case 0xe0:
                return midi_msg_type::pitch_bend;

            default:
                return midi_msg_type::none;
        }
    }

    // Get MIDI message type as text
    [[nodiscard]] const char * get_type_text() const
    {
        switch (status & 0xf0) {
            case 0xb0:
                return "Control Change";

            case 0x90:
                return "Note On";

            case 0x80:
                return "Note Off";

            case 0xc0:
                return "Program Change";

            case 0xa0:
                return "Aftertouch";

            case 0xd0:
                return "Channel Pressure";

            case 0xe0:
                return "Pitch Bend";

            default:
                return "<unknown>";
        }
    }

    // Get MIDI message type as code
    [[nodiscard]] const char * get_type_code() const
    {
        switch (status & 0xf0) {
            case 0xb0:
                return KEY_CONTROL_CHANGE;

            case 0x90:
            case 0x80:
                return KEY_NOTE;

            case 0xc0:
                return KEY_PROGRAM_CHANGE;

            default:
                return "";
        }
    }

    // get a string containing the channel, type and data
    [[nodiscard]] std::string get_key() const
    {
        return utils::create_map_key(get_channel(), get_type_code(), data);
    }
};

} // Namespace xmidictrl

#endif // MIDI_MESSAGE_H