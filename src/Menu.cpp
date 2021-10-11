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

// Standard
#include <string>

// X-Plane Environment
#include "PluginLogger.h"

// XMidiCtrl
#include "Config.h"
#include "Menu.h"
#include "Plugin.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Menu::Menu() {
    m_menuContainer = 0;
    m_menuId = nullptr;
}


/**
 * Destructor
 */
Menu::~Menu() {
    deleteMenu();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create plugin menu
 */
void Menu::createMenu() {
    m_menuContainer = XPLMAppendMenuItem(XPLMFindPluginsMenu(), XMIDICTRL_NAME, nullptr, 0);
    m_menuId = XPLMCreateMenu(XMIDICTRL_NAME, XPLMFindPluginsMenu(), m_menuContainer, menuHandler, this);

    XPLMAppendMenuItem(m_menuId, "Search for MIDI Devices", (void*) MENUITEM_SEARCH_MIDI_DEVICES, 0);
    XPLMAppendMenuSeparator(m_menuId);
    XPLMAppendMenuItem(m_menuId, "Show Aircraft Profile", (void*) MENUITEM_SHOW_AIRCRAFT_PROFILE, 0);
    XPLMAppendMenuItem(m_menuId, "Reload Aircraft Profile", (void*) MENUITEM_RELOAD_AIRCRAFT_PROFILE, 0);
    XPLMAppendMenuSeparator(m_menuId);
    XPLMAppendMenuItem(m_menuId, "Settings", (void*) MENUITEM_SETTINGS_DIALOG, 0);
    XPLMAppendMenuSeparator(m_menuId);
    XPLMAppendMenuItem(m_menuId, std::string("About " + std::string(XMIDICTRL_NAME)).c_str(),
                       (void*) MENUITEM_ABOUT_DIALOG, 0);
}


/**
 * Delete the menu items
 */
void Menu::deleteMenu() {
    m_menuContainer = 0;
    m_menuId = nullptr;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Event handler
 */
void Menu::menuHandler(void* in_menu_ref, void* in_item_ref) {
    if (!strcmp((const char*) in_item_ref, MENUITEM_SEARCH_MIDI_DEVICES)) {
        LOG_INFO << "MENU :: Menu Handler -> MENUITEM_SEARCH_MIDI_DEVICES" << LOG_END
        Plugin::Instance().showMidiDevicesDialog();
    }
    else if (!strcmp((const char*) in_item_ref, MENUITEM_RELOAD_AIRCRAFT_PROFILE)) {
        LOG_INFO << "MENU :: Menu Handler -> MENUITEM_RELOAD_AIRCRAFT_PROFILE" << LOG_END
        Plugin::Instance().loadAircraftProfile();
    }
    else if (!strcmp((const char*) in_item_ref, MENUITEM_SETTINGS_DIALOG)) {
        LOG_INFO << "MENU :: Menu Handler -> MENUITEM_SETTINGS_DIALOG" << LOG_END
        Plugin::Instance().showSettingsDialog();
    }
    else if (!strcmp((const char*) in_item_ref, MENUITEM_ABOUT_DIALOG)) {
        LOG_INFO << "MENU :: Menu Handler -> MENUITEM_ABOUT_DIALOG" << LOG_END
        Plugin::Instance().showAboutDialog();
    }
}

} // Namespace XMidiCtrl