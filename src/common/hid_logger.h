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

#ifndef XMC_HID_LOGGER_H
#define XMC_HID_LOGGER_H

// Standard
#include <deque>
#include <memory>

// XMidiCtrl
#include "hid_event.h"

namespace xmidictrl {

class hid_logger {
public:
	explicit hid_logger(bool in_enabled, int in_max_messages);
	~hid_logger() = default;

	void clear();
	size_t count();

	void enable();
	void disable();

	void set_max_events(int in_max_events);

	hid_event* event(int in_index);

	void add(const std::shared_ptr<hid_event>& in_evt);

private:
	void adjust_log_size();

	bool m_enabled;
	int m_max_events;

	std::deque<std::shared_ptr<hid_event>> m_events;
};

} // Namespace xmidictrl

#endif // XMC_HID_LOGGER_H