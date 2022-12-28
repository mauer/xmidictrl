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
#include <vector>

// XMidiCtrl
#include "map_in.h"

namespace xmidictrl {

class map_in_list {
public:
    explicit map_in_list() = default;
    ~map_in_list();

    void add(const std::shared_ptr<map_in>& in_map);
    std::vector<std::shared_ptr<map_in>> find(const std::string &in_key);

    unsigned int size();

protected:
    std::multimap<std::string, std::shared_ptr<map_in>> m_list {};
};

} // Namespace xmidictrl
