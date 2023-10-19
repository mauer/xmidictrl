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

#ifndef XMC_OUTBOUND_TASK_H
#define XMC_OUTBOUND_TASK_H

// Standard
#include <memory>

// XMidiCtrl
#include "map.h"
#include "midi_message.h"

namespace xmidictrl {

struct outbound_task {
    bool data_changed {false};

    midi_msg_type type {midi_msg_type::none};

    unsigned char channel {MIDI_NONE};
    unsigned char data {MIDI_NONE};

    unsigned char velocity {MIDI_NONE};

    std::shared_ptr<map> mapping;
};

} // Namespace xmidictrl

#endif // XMC_OUTBOUND_TASK_H
