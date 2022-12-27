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

#pragma once

// Standard
#include <memory>
#include <map>

// XMidiCtrl
#include "map_init.h"

namespace xmidictrl {

typedef std::vector<std::shared_ptr<map_init>>::iterator map_init_itr;

class map_init_list {
public:
    explicit map_init_list() = default;
    ~map_init_list();

    void add(const std::shared_ptr<map_init> &map);

    map_init_itr begin();
    map_init_itr end();

    unsigned int size();

protected:
    std::vector<std::shared_ptr<map_init>> m_list;
};

} // Namespace xmidictrl
