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

#include "menu.h"

// Standard
#include <string>

// XMidiCtrl
#include "plugin.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
menu::~menu()
{
    remove_menu();
}




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

    XPLMAppendMenuItem(m_menu_id, "Show MIDI Devices", (void *) MENUITEM_DEVICES_WINDOW, 0);
    XPLMAppendMenuItem(m_menu_id, "Show Messages", (void *) MENUITEM_MESSAGES_WINDOW, 0);

    XPLMAppendMenuSeparator(m_menu_id);
    XPLMAppendMenuItem(m_menu_id, "Show Aircraft Profile", (void*) MENUITEM_PROFILE_WINDOW, 0);

    XPLMAppendMenuItem(m_menu_id, "Reload Aircraft Profile", (void *) MENUITEM_RELOAD_PROFILE, 0);

    XPLMAppendMenuSeparator(m_menu_id);

    XPLMAppendMenuItem(m_menu_id, "Settings", (void *) MENUITEM_SETTINGS_WINDOW, 0);

    XPLMAppendMenuSeparator(m_menu_id);

    XPLMAppendMenuItem(m_menu_id, "Documentation", (void *) MENUITEM_SHOW_DOCUMENTATION, 0);
    XPLMAppendMenuItem(m_menu_id, "Support", (void *) MENUITEM_SHOW_SUPPORT, 0);

    XPLMAppendMenuSeparator(m_menu_id);

    XPLMAppendMenuItem(m_menu_id, std::string_view("About " + std::string(XMIDICTRL_NAME)).data(),
                       (void *) MENUITEM_ABOUT_WINDOW, 0);
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
void menu::event_handler([[maybe_unused]] void *in_menu_ref, void *in_item_ref)
{
    if (!strcmp((const char *) in_item_ref, MENUITEM_DEVICES_WINDOW))
        plugin::instance().show_devices_window();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_MESSAGES_WINDOW))
        plugin::instance().show_messages_window();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_PROFILE_WINDOW))
        plugin::instance().show_profile_window();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_RELOAD_PROFILE))
        plugin::instance().load_profile();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_SETTINGS_WINDOW))
        plugin::instance().show_settings_window();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_SHOW_DOCUMENTATION))
        show_documentation();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_SHOW_SUPPORT))
        show_support();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_ABOUT_WINDOW))
        plugin::instance().show_about_window();
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
void menu::show_support()
{
    std::string linkChar = "https://forums.x-pilot.com/forums/forum/274-xmidictrl/";

#if IBM
    ShellExecute(nullptr, nullptr, linkChar.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif LIN
    if (system((std::string("xdg-open '") + linkChar + "'").c_str())) {}
#endif
}

} // Namespace XMidiCtrl
