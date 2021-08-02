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


// XMidiCtrl
#include "Plugin.h"
#include "Menu.h"


using namespace XMidiCtrl;


XMidiCtrlMenu::XMidiCtrlMenu() {
    m_menu_container = 0;
}


void XMidiCtrlMenu::createMenu() {
    m_menu_container = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "XMidiCtrl", 0, 0);
    m_menu_id = XPLMCreateMenu("XMidiCtrl", XPLMFindPluginsMenu(), m_menu_container, MenuHandler, this);
    
    XPLMAppendMenuItem(m_menu_id, "MIDI Watcher", (void *)1, 1);
    XPLMAppendMenuItem(m_menu_id, "Reload Settings", (void *)"reload_settings", 1);
    XPLMAppendMenuSeparator(m_menu_id);
    XPLMAppendMenuItem(m_menu_id, "About XMidiCtrl", (void *)"about_dialog", 1);
}


void XMidiCtrlMenu::deleteMenu() {
	XPLMDestroyMenu(m_menu_id);
    XPLMRemoveMenuItem(XPLMFindPluginsMenu(), m_menu_container);
}


void XMidiCtrlMenu::MenuHandler(void* in_menu_ref, void* in_item_ref) {
    if (!strcmp((const char*) in_item_ref, "reload_settings")) {
        Plugin::Instance().reloadSettings();
    }

    if (!strcmp((const char*) in_item_ref, "about_dialog")) {
        Plugin::Instance().showAboutDialog();
    }
}