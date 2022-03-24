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

#ifndef MIDI_LOG_MSG_H
#define MIDI_LOG_MSG_H

// Standard
#include <string>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

struct midi_log_msg {
    std::string time {};
    std::string direction {};

    unsigned int port {0};

    std::string type {};

    unsigned char channel {MIDI_NONE};

    unsigned char status {MIDI_NONE};
    unsigned char data_1 {MIDI_NONE};
    unsigned char data_2 {MIDI_NONE};
};

} // Namespace xmidictrl

#endif // MIDI_LOG_MSG_H