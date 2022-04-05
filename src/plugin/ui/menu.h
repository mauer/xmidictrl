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

#ifndef MENU_H
#define MENU_H

// Standard
#include <memory>

// X-Plane SDK
#include "XPLMMenus.h"

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

class menu {
public:
    menu() = default;
    ~menu();

    void create_menu();
    void remove_menu();

private:
    static void event_handler([[maybe_unused]] void *in_menu_ref, void *in_item_ref);

    static void show_documentation();
    static void show_support();

    int m_menu_container = -1;
    XPLMMenuID m_menu_id = nullptr;
};

} // Namespace xmidictrl

#endif // MENU_H