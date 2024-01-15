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

#include "menu.h"

// Standard
#include <string>

// XMidiCtrl
#include "plugin.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create the plugin menu
 */
void menu::create_menu()
{
	m_menu_container = XPLMAppendMenuItem(XPLMFindPluginsMenu(), XMIDICTRL_NAME, nullptr, 0);
	m_menu_id = XPLMCreateMenu(XMIDICTRL_NAME, XPLMFindPluginsMenu(), m_menu_container, event_handler, this);

	XPLMAppendMenuItem(m_menu_id, "Show MIDI Devices", (void*) menu_show_devices_window, 0);

	XPLMAppendMenuSeparator(m_menu_id);

	XPLMAppendMenuItem(m_menu_id, "Show Log Viewer", (void*) menu_show_log_viewer, 0);
	XPLMAppendMenuItem(m_menu_id, "Show MIDI Watcher", (void*) menu_show_midi_watcher, 0);

	XPLMAppendMenuSeparator(m_menu_id);

	XPLMAppendMenuItem(m_menu_id, "Show Aircraft Profile", (void*) menu_show_profile_window, 0);
	XPLMAppendMenuItem(m_menu_id, "Reload Aircraft Profile", (void*) menu_reload_profile, 0);

	XPLMAppendMenuSeparator(m_menu_id);

	XPLMAppendMenuItem(m_menu_id, "Settings", (void*) menu_show_settings_window, 0);

	XPLMAppendMenuSeparator(m_menu_id);

	XPLMAppendMenuItem(m_menu_id, "Documentation", (void*) menu_show_documentation, 0);
	XPLMAppendMenuItem(m_menu_id, "Support", (void*) menu_show_support_forum, 0);

	XPLMAppendMenuSeparator(m_menu_id);

	XPLMAppendMenuItem(m_menu_id,
					   std::string_view("About " + std::string(XMIDICTRL_NAME)).data(),
					   (void*) menu_show_about_window,
					   0);
}


/**
 * Remove all menu items
 */
void menu::remove_menu()
{
	if (m_menu_container > -1)
		XPLMRemoveMenuItem(XPLMFindPluginsMenu(), m_menu_container);

	if (m_menu_id != nullptr)
		XPLMDestroyMenu(m_menu_id);

	m_menu_container = -1;
	m_menu_id = nullptr;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Event handler fpr the menu
 */
void menu::event_handler(void*, void* in_item_ref)
{
	auto cmd = (intptr_t) in_item_ref;

	switch (cmd) {
		case menu_show_devices_window:
			plugin::instance().show_devices_window();
			break;

		case menu_show_log_viewer:
			plugin::instance().show_log_viewer();
			break;

		case menu_show_midi_watcher:
			plugin::instance().show_midi_watcher();
			break;

		case menu_show_profile_window:
			plugin::instance().show_profile_window();
			break;

		case menu_reload_profile:
			plugin::instance().load_profile();
			break;

		case menu_show_settings_window:
			plugin::instance().show_settings_window();
			break;

		case menu_show_documentation:
			show_documentation();
			break;

		case menu_show_support_forum:
			show_support_forum();
			break;

		case menu_show_about_window:
			plugin::instance().show_about_window();
			break;

		default:
			break;
	}
}


/**
 * Open the plugin documentation
 */
void menu::show_documentation()
{
	std::string linkChar = "https://mauer.github.io/xmidictrl/#/";

#if IBM
	ShellExecute(nullptr, nullptr, linkChar.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif LIN
	if (system((std::string("xdg-open '") + linkChar + "'").c_str())) {}
#endif
}


/**
 * Open the plugin support forum
 */
void menu::show_support_forum()
{
	std::string linkChar = "https://forums.x-pilot.com/forums/forum/274-xmidictrl/";

#if IBM
	ShellExecute(nullptr, nullptr, linkChar.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif LIN
	if (system((std::string("xdg-open '") + linkChar + "'").c_str())) {}
#endif
}

} // namespace xmidictrl
