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
    unsigned char data_1 {MIDI_NONE};
    unsigned char data_2 {MIDI_NONE};

    // Return the direction as text
    [[nodiscard]] std::string direction_as_text() const
    {
        if (direction == msg_direction::inbound)
            return "Inbound";
        else
            return "Outbound";
    }

    // Get the channel
    [[nodiscard]] unsigned char get_channel() const
    {
        if ((status & 0xf0) != 0xf0)
            return (status & 0xf) + 1;
        else
            return MIDI_NONE;
    }

    // Get the MIDI message type
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

    // Get the MIDI message type as text
    [[nodiscard]] std::string get_type_as_text() const
    {
        switch (get_type()) {
            case midi_msg_type::control_change:
                return "Control Change";

            case midi_msg_type::note_on:
                return "Note On";

            case midi_msg_type::note_off:
                return "Note Off";

            case midi_msg_type::program_change:
                return "Program Change";

            case midi_msg_type::aftertouch:
                return "Aftertouch";

            case midi_msg_type::channel_pressure:
                return "Channel Pressure";

            case midi_msg_type::pitch_bend:
                return "Pitch Bend";

            default:
                break;
        }

        return "<unknown";
    }

    // Get the MIDI message type as code
    [[nodiscard]] std::string get_type_code() const
    {
        switch (get_type()) {
            case midi_msg_type::control_change:
                return KEY_CONTROL_CHANGE;

            case midi_msg_type::note_on:
            case midi_msg_type::note_off:
                return KEY_NOTE;

            case midi_msg_type::program_change:
                return KEY_PROGRAM_CHANGE;

            case midi_msg_type::pitch_bend:
                return KEY_PITCH_BEND;

            default:
                break;
        }

        return "";
    }

    // Get a string containing the channel, type and data
    [[nodiscard]] std::string get_key() const
    {
        // for pitch bend messages the data will be ignored, it's always 0
        if (get_type() == midi_msg_type::pitch_bend)
            return utils::create_map_key(get_channel(), get_type_code(), 0);
        else
            return utils::create_map_key(get_channel(), get_type_code(), data_1);
    }
};

} // Namespace xmidictrl

#endif // MIDI_MESSAGE_H