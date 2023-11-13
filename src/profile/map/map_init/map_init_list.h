//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#ifndef XMC_MAP_INIT_LIST_H
#define XMC_MAP_INIT_LIST_H

// Standard
#include <memory>
#include <map>

// XMidiCtrl
#include "map_init.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

typedef std::vector<std::shared_ptr<map_init>>::iterator map_init_itr;




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_init_list {
public:
    explicit map_init_list() = default;
    ~map_init_list();

    void add(const std::shared_ptr<map_init>& in_map);

    map_init_itr begin();
    map_init_itr end();

    size_t size();

private:
    unsigned int m_last_map_no {0};

    std::vector<std::shared_ptr<map_init>> m_list;
};

} // Namespace xmidictrl

#endif // XMC_MAP_INIT_LIST_H
