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

#ifndef XMC_MENU_H
#define XMC_MENU_H

// Standard
#include <string>

// X-Plane SDK
#include "XPLMMenus.h"

namespace xmidictrl {

class menu {
public:
	menu() = default;
	~menu() = default;

	void create_menu();
	void remove_menu();

private:
	enum {
		menu_show_devices_window,
		menu_show_log_viewer,
		menu_show_midi_watcher,
		menu_show_profile_window,
		menu_reload_profile,
		menu_show_settings_window,
		menu_show_documentation,
		menu_show_support_forum,
		menu_show_about_window
	};

	static void event_handler(void* in_menu_ref, void* in_item_ref);

	static void show_documentation();
	static void show_support_forum();

	int m_menu_container = -1;
	XPLMMenuID m_menu_id = nullptr;
};

} // Namespace xmidictrl

#endif // XMC_MENU_H