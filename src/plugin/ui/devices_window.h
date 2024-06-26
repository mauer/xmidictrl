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

#ifndef XMC_DEVICES_WINDOW_H
#define XMC_DEVICES_WINDOW_H

// Standard
#include <memory>
#include <map>

// RtMidi
#include "RtMidi.h"

// XMidiCtrl
#include "hid_device_data.h"
#include "environment.h"
#include "imgui_window.h"

namespace xmidictrl {

class devices_window : public imgui_window {
public:
    devices_window(text_logger& in_log, environment& in_env);
    ~devices_window() override = default;

protected:
    void create_widgets() override;

private:
	// functions
	std::map<std::string, std::unique_ptr<hid_device_data>> read_hid_devices();

	void create_tab_midi();
	void create_tab_hid();

	// members
	std::unique_ptr<RtMidiIn> m_midi_in {nullptr};
    std::unique_ptr<RtMidiOut> m_midi_out {nullptr};

	std::map<std::string, std::unique_ptr<hid_device_data>> m_hid_devices {};
};

} // Namespace xmidictrl

#endif // XMC_DEVICES_WINDOW_H
