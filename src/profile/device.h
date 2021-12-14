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

#ifndef DEVICE_H
#define DEVICE_H

// Standard
#include <chrono>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>

// RtMidi
#include "RtMidi.h"

// XMidiCtrl
#include "map_in.h"
#include "map_in_list.h"
#include "map_out.h"
#include "map_out_list.h"
#include "types.h"

namespace xmidictrl {

class device_list;

class device {
public:
    device(std::string_view name, unsigned int port_in, unsigned int port_out, std::shared_ptr<device_list> device_list);
    ~device();

    // no copying or copy assignments are allowed
    device(device const &) = delete;
    device &operator=(device const &) = delete;

    void add_inbound_map(std::shared_ptr<map_in> &mapping);
    void add_outbound_map(std::shared_ptr<map_out> &mapping);

    bool open_connections();
    void close_connections();

    static void midi_callback(double deltatime, std::vector<unsigned char> *message, void *userdata);
    void process_inbound_message(double deltatime, std::vector<unsigned char> *message);

    void process_outbound_mappings();

private:
    void save_event_datetime(unsigned int cc);
    double retrieve_event_datetime(unsigned int cc);

    std::shared_ptr<device_list> m_device_list;

    std::string m_name;
    unsigned int m_port_in;
    unsigned int m_port_out;

    map_in_list m_map_in;
    map_out_list m_map_out;

    std::unique_ptr<RtMidiIn> m_midi_in;
    std::unique_ptr<RtMidiOut> m_midi_out;

    std::map<int, time_point> m_event_storage;
    std::set<int> m_cc_locked;
};

} // Namespace xmidictrl

#endif // DEVICE_H