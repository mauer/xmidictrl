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

#ifndef MAP_OUT_LIST_H
#define MAP_OUT_LIST_H

// Standard
#include <memory>
#include <map>

// XMidiCtrl
#include "map_out.h"

namespace xmidictrl {

typedef std::multimap<std::string, map_out::ptr>::iterator map_out_itr;
typedef std::pair<map_out_itr, map_out_itr> map_out_pair;

class map_out_list {
public:
    explicit map_out_list() = default;
    ~map_out_list();

    void add(const std::shared_ptr<map_out>& map);
    map_out_pair get(int ch, int cc);

    map_out_itr begin();
    map_out_itr end();

    unsigned int size();

protected:
    std::multimap<std::string, map_out::ptr> m_list;
};

} // Namespace xmidictrl

#endif // MAP_OUT_LIST_H
