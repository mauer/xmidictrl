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

#include "map_in_list.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
map_in_list::~map_in_list()
{
    m_list_key.clear();
    m_list_no.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a new mapping
 */
void map_in_list::add(const std::shared_ptr<map_in>& in_map)
{
    m_last_map_no++;
    in_map->set_no(m_last_map_no);

    m_list_key.insert({in_map->get_key(), in_map});
    m_list_no.insert({in_map->no(), in_map});
}


/**
 * Retrieve all mappings for a given key
 */
std::vector<std::shared_ptr<map_in>> map_in_list::find(const std::string& in_key)
{
    std::vector<std::shared_ptr<map_in>> result {};

    auto search_result = m_list_key.equal_range(in_key);

    for (auto it = search_result.first; it != search_result.second; it++)
        result.push_back(it->second);

    return result;
}


/**
 * Return the number of mapping
 */
size_t map_in_list::size()
{
    return m_list_key.size();
}


/**
 * Return the begin iterator
 */
std::multimap<unsigned int, std::shared_ptr<map_in>>::iterator map_in_list::begin()
{
    return m_list_no.begin();
}


/**
 * Return the end iterator
 */
std::multimap<unsigned int, std::shared_ptr<map_in>>::iterator map_in_list::end()
{
    return m_list_no.end();
}

} // Namespace xmidictrl