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
