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

namespace xmidictrl {

struct midi_message {
    time_point time {time_point::min()};
    midi_type type {midi_type::inbound};

    unsigned int port {0};

    unsigned int status {0};
    unsigned int data {0};
    unsigned int velocity {0};
};

} // Namespace xmidictrl

#endif // MIDI_MESSAGE_H