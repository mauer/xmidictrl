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

#ifndef XMC_HID_DEVICE_H
#define XMC_HID_DEVICE_H

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
#include "hid_logger.h"
#include "map_in.h"
#include "map_in_list.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class hid_device : public device {
public:
	hid_device(text_logger& in_text_log,
			   hid_logger& in_hid_log,
			   environment& in_env,
			   std::unique_ptr<device_settings> in_settings);
	~hid_device() override = default;

	// no copying or copy assignments are allowed
	hid_device(hid_device const&) = delete;
	hid_device& operator=(hid_device const&) = delete;

	device_type type() override;

	void process_inbound_event();

private:
	// members
	hid_logger& m_hid_log;
};

} // Namespace xmidictrl

#endif // XMC_VIRTUAL_MIDI_DEVICE_H
