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

#include "map_out_list.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
map_out_list::~map_out_list()
{
    m_list.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a new mapping
 */
void map_out_list::add(const std::shared_ptr<map_out> &in_map)
{
    m_last_map_no++;
    in_map->set_no(m_last_map_no);

    m_list.push_back(in_map);
}


/**
 * Return the begin iterator
 */
map_out_itr map_out_list::begin()
{
    return m_list.begin();
}


/**
 * Return the end iterator
 */
map_out_itr map_out_list::end()
{
    return m_list.end();
}


/**
 * Return the number of mapping
 */
size_t map_out_list::size()
{
    return m_list.size();
}

} // Namespace xmidictrl