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

#include "map_init_list.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
map_init_list::~map_init_list()
{
    m_list.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------


/**
 * Create all init mappings for a device and store it
 */
void map_init_list::create_mappings(text_logger& in_log,
                                    toml::array in_profile,
                                    device_settings& in_dev_settings,
                                    std::string_view in_inc_name)
{
    auto dev_no = std::to_string(in_dev_settings.device_no);

    in_log.info("Device " + dev_no + " :: "
                     + std::to_string(in_profile.size()) + " init mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_profile.size()); map_no++) {
        std::shared_ptr<map_init> mapping;

        in_log.debug("Device " + dev_no + " :: Mapping " + std::to_string(map_no)
                          + " :: Reading config");

        try {
            mapping = std::make_shared<map_init>();

            // read the settings and check if everything we need was defined
            mapping->read_config(in_log, in_profile[map_no]);
            mapping->set_include_name(in_inc_name);

            if (mapping->check(in_log)) {
                add(mapping);
                in_log.debug(" --> Mapping added");
            } else {
                in_log.error(
                    "Device " + dev_no + " :: Mapping " + std::to_string(map_no)
                    + " :: Parameters incomplete or incorrect");
            }
        } catch (toml::type_error& error) {
            in_log.error("Device " + dev_no + " :: Mapping " + std::to_string(map_no)
                              + " :: Error reading mapping");
            in_log.error(error.what());
        }
    }
}


/**
 * Return the begin iterator
 */
map_init_itr map_init_list::begin()
{
    return m_list.begin();
}


/**
 * Return the end iterator
 */
map_init_itr map_init_list::end()
{
    return m_list.end();
}


/**
 * Return the number of mapping
 */
size_t map_init_list::size()
{
    return m_list.size();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a new mapping
 */
void map_init_list::add(const std::shared_ptr<map_init>& in_map)
{
    m_last_map_no++;
    in_map->set_no(m_last_map_no);

    m_list.push_back(in_map);
}

} // Namespace xmidictrl