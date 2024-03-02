//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

#ifndef XMC_MAP_RESULT_H
#define XMC_MAP_RESULT_H

// XMidiCtrl
#include "midi_message.h"

namespace xmidictrl {

struct map_result {
	// result for inbound messages
	bool completed;

	// result for outbound messages
	bool data_changed {false};

	midi_msg_type type {midi_msg_type::none};

	char channel {MIDI_NONE};
	char data_1 {MIDI_NONE};
	char data_2 {MIDI_NONE};
};

} // Namespace xmidictrl

#endif // XMC_MAP_RESULT_H
