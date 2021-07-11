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
    XPLMAppendMenuItem(m_menu_id, "About XMidiCtrl", (void *)1, 1);
}


void XMidiCtrlMenu::deleteMenu() {
	XPLMDestroyMenu(m_menu_id);
    XPLMRemoveMenuItem(XPLMFindPluginsMenu(), m_menu_container);
}


void XMidiCtrlMenu::MenuHandler(void* in_menu_ref, void* in_item_ref) {
    if (!strcmp((const char*) in_item_ref, "reload_settings")) {
        Plugin::Instance().reloadSettings();
    }
}