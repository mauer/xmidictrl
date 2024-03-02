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

#include "hid.h"

// fmt
#include "fmt/format.h"

// HID Api
#include "hidapi.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Returns all list of all connected HID devices
 * @return List of all HID devices with detailed information
 */
std::vector<std::unique_ptr<hid_device_data>> hid::read_devices()
{
	auto devices = std::vector<std::unique_ptr<hid_device_data>>();
	struct hid_device_info* hid_devs;

	hid_devs = hid_enumerate(0x0, 0x0);

	for (; hid_devs; hid_devs = hid_devs->next) {
		if (hid_devs->interface_number == -1)
			continue;

		auto dev = std::make_unique<hid_device_data>();

		// convert manufacturer string
		auto ws = std::wstring(hid_devs->manufacturer_string);
		dev->manufacturer_string = std::string(ws.begin(), ws.end());

		// convert product string
		ws = std::wstring(hid_devs->product_string);
		dev->product_string = std::string(ws.begin(), ws.end());

		dev->vendor_id = hid_devs->vendor_id;
		dev->product_id = hid_devs->product_id;

		devices.push_back(std::move(dev));
	}

	hid_free_enumeration(hid_devs);

	return devices;
}

} // Namespace xmidictrl