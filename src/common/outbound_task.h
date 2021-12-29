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

#ifndef OUTBOUND_TASK_H
#define OUTBOUND_TASK_H

// Standard
#include <memory>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"

namespace xmidictrl {

struct outbound_task {
    bool data_changed {false};

    int ch {-1};
    int cc {-1};
    int velocity {-1};

    std::shared_ptr<midi_message> msg;
};

} // Namespace xmidictrl

#endif // OUTBOUND_TASK_H