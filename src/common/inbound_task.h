
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

#ifndef XMC_INBOUND_TASK_H
#define XMC_INBOUND_TASK_H

// Standard
#include <memory>
#include <string>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"

namespace xmidictrl {

struct inbound_task {
    std::shared_ptr<midi_message> msg;
    std::string sl_value;
    std::shared_ptr<map_in> map;
};

} // Namespace xmidictrl

#endif // XMC_INBOUND_TASK_H