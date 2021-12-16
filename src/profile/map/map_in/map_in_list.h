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

#ifndef MAP_IN_LIST_H
#define MAP_IN_LIST_H

// Standard
#include <memory>
#include <map>

// XMidiCtrl
#include "map_in.h"

namespace xmidictrl {

typedef std::multimap<std::string, map_in::ptr>::iterator map_in_itr;
typedef std::pair<map_in_itr, map_in_itr> map_in_pair;

class map_in_list {
public:
    explicit map_in_list() = default;
    ~map_in_list();

    void add(const std::shared_ptr<map_in>& map);
    map_in_pair get(int ch, int cc);

    map_in_itr begin();
    map_in_itr end();

    unsigned int size();

protected:
    std::multimap<std::string, map_in::ptr> m_list;
};

} // Namespace xmidictrl

#endif // MAP_IN_LIST_H
