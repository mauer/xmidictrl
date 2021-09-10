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

#ifndef MENU_H
#define MENU_H

// X-Plane SDK
#include "XPLMMenus.h"

namespace XMidiCtrl {

const char* const MENUITEM_SEARCH_MIDI_DEVICES = "SEARCH_MIDI_DEVICES";
const char* const MENUITEM_SETTINGS_DIALOG     = "SETTINGS_DIALOG";
const char* const MENUITEM_RELOAD_SETTINGS     = "RELOAD_SETTINGS";
const char* const MENUITEM_ABOUT_DIALOG        = "ABOUT_DIALOG";

class Menu {
public:
    Menu();
    ~Menu();

    void createMenu();
    void deleteMenu();

private:
    static void menuHandler(void* in_menu_ref, void* in_item_ref);

    int m_menuContainer;
    XPLMMenuID m_menuId;
};

} // Namespace XMidiCtrl

#endif // MENU_H