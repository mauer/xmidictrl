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

#ifndef XMC_VIRTUAL_MIDI_DEVICE_H
#define XMC_VIRTUAL_MIDI_DEVICE_H

// Standard
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <string_view>
#include <text_logger.h>
#include <thread>

// XMidiCtrl
#include "device.h"
#include "map_in.h"
#include "map_in_list.h"
#include "midi_logger.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class virtual_midi_device : public device {
  public:
	virtual_midi_device(text_logger& in_text_log,
						midi_logger& in_midi_log,
						environment& in_env,
						std::unique_ptr<device_settings> in_settings);
	~virtual_midi_device() override = default;

	// no copying or copy assignments are allowed
	virtual_midi_device(virtual_midi_device const&) = delete;
	virtual_midi_device& operator=(virtual_midi_device const&) = delete;

	device_type type() override;

	void process_inbound_message(unsigned char in_channel, unsigned char in_data, unsigned char in_velocity);
};

} // Namespace xmidictrl

#endif // XMC_VIRTUAL_MIDI_DEVICE_H
