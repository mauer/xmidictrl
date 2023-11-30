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

#ifndef XMC_MAP_OUT_LIST_H
#define XMC_MAP_OUT_LIST_H

// Standard
#include <memory>
#include <map>

// XMidiCtrl
#include "map_out.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

typedef std::vector<std::shared_ptr<map_out>>::iterator map_out_itr;




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_out_list {
public:
    explicit map_out_list() = default;
    ~map_out_list();

    void create_mappings(text_logger& in_log,
                         toml::array in_profile,
                         environment& in_env,
                         device_settings& in_dev_settings,
                         std::string_view in_inc_name);

    map_out_itr begin();
    map_out_itr end();

    size_t size();

private:
    map_out_type read_map_type(text_logger& in_log, toml::value& in_profile);
    static map_out_type translate_map_type(std::string_view in_type_str);

    void add(const std::shared_ptr<map_out>& in_map);

    // constants
    static constexpr std::string_view c_cfg_type {"type"};

    // members
    unsigned int m_last_map_no {0};

    std::vector<std::shared_ptr<map_out>> m_list;
};

} // Namespace xmidictrl

#endif // XMC_MAP_OUT_LIST_H
