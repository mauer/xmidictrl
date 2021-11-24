//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#include "menu.h"

// Standard
#include <string>

// X-Plane Environment
#include "logger.h"

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

    XPLMAppendMenuItem(m_menu_id, "Search for MIDI Devices", (void *) MENUITEM_MIDI_DEVICES_DIALOG, 0);
    XPLMAppendMenuItem(m_menu_id, "Show messages", (void *) MENUITEM_MESSAGES_DIALOG, 0);

    XPLMAppendMenuSeparator(m_menu_id);
    //XPLMAppendMenuItem(m_menuId, "Show Aircraft Profile", (void*) MENUITEM_SHOW_AIRCRAFT_PROFILE, 0);

    XPLMAppendMenuItem(m_menu_id, "Reload Aircraft Profile", (void *) MENUITEM_RELOAD_AIRCRAFT_PROFILE, 0);

    XPLMAppendMenuSeparator(m_menu_id);

    XPLMAppendMenuItem(m_menu_id, "Settings", (void *) MENUITEM_SETTINGS_DIALOG, 0);

    XPLMAppendMenuSeparator(m_menu_id);

    XPLMAppendMenuItem(m_menu_id, std::string_view("About " + std::string(XMIDICTRL_NAME)).data(),
                       (void *) MENUITEM_ABOUT_DIALOG, 0);
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
void menu::event_handler(void *in_menu_ref, void *in_item_ref)
{
    if (!strcmp((const char *) in_item_ref, MENUITEM_MIDI_DEVICES_DIALOG))
        plugin::instance().show_devices_dialog();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_MESSAGES_DIALOG))
        plugin::instance().show_messages_dialog();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_SETTINGS_DIALOG))
        plugin::instance().show_settings_dialog();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_RELOAD_AIRCRAFT_PROFILE))
        plugin::instance().load_profile();
    else if (!strcmp((const char *) in_item_ref, MENUITEM_ABOUT_DIALOG))
        plugin::instance().show_about_dialog();
}

} // Namespace XMidiCtrl