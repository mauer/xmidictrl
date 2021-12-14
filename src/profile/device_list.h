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

#ifndef DEVICE_LIST_H
#define DEVICE_LIST_H

// Standard
#include <memory>
#include <vector>
#include <queue>

// XMidiCtrl
#include "device.h"
#include "task.h"

namespace xmidictrl {

class device_list : public std::enable_shared_from_this<device_list> {
public:
    explicit device_list() = default;
    ~device_list();

    typedef std::shared_ptr<device_list> ptr;

    std::shared_ptr<device> create_device(std::string_view name, unsigned int port_in, unsigned int port_out);

    bool open_connections();
    void close_connections();

    void add_event(const std::shared_ptr<task> &event);

    void process_inbound_events();
    void process_outbound_mappings();

    void clear();
    unsigned int size();

private:
    std::queue<std::shared_ptr<task>> m_inbound_events;
    std::vector<std::shared_ptr<device>> m_device_list;
};

} // Namespace xmidictrl

#endif // DEVICE_LIST_H