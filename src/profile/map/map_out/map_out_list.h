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
