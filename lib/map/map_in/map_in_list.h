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

#ifndef XMC_MAP_IN_LIST_H
#define XMC_MAP_IN_LIST_H

// Standard
#include <memory>
#include <map>
#include <vector>

// XMidiCtrl
#include "device_settings.h"
#include "map_in.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in_list {
public:
    explicit map_in_list() = default;

    void create_mappings(text_logger& in_log,
                         toml::array in_profile,
                         environment& in_env,
                         bool in_is_virtual,
                         const device_settings& in_dev_settings,
                         std::string_view in_inc_name,
                         toml::value& in_config);

    std::vector<std::shared_ptr<map_in>> find(const std::string &in_key);

    size_t size() const;

    std::multimap<unsigned int, std::shared_ptr<map_in>>::iterator begin();
    std::multimap<unsigned int, std::shared_ptr<map_in>>::iterator end();

private:
    map_in_type read_map_type(text_logger& in_log, toml::value& in_params);
    map_in_type translate_map_type(std::string_view in_type_str) const;

    void add(const std::shared_ptr<map_in>& in_map);

    // constants
    static constexpr std::string_view c_cfg_type {"type"};

    // members
    unsigned int m_last_map_no {0};

    std::multimap<std::string, std::shared_ptr<map_in>> m_list_key {};
    std::map<unsigned int, std::shared_ptr<map_in>> m_list_no {};
};

} // Namespace xmidictrl

#endif // XMC_MAP_IN_LIST_H
