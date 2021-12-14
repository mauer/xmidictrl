//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#include "map_out_list.h"

// XMidiCtrl
#include "utils.h"

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
void map_out_list::add(const std::shared_ptr<map_out>& map)
{
    m_list.insert({utils::ch_cc(map->ch(), map->cc()), map});
}


/**
 * Retrieve the mapping for a control change
 */
map_out_pair map_out_list::get(const int ch, const int cc)
{
    return m_list.equal_range(utils::ch_cc(ch, cc));
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
unsigned int map_out_list::size()
{
    return m_list.size();
}

} // Namespace xmidictrl