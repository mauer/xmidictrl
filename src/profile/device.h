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

#ifndef DEVICE_H
#define DEVICE_H

// Standard
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <queue>

// RtMidi
#include "RtMidi.h"

// XMidiCtrl
#include "map_in.h"
#include "map_in_list.h"
#include "map_out.h"
#include "map_out_list.h"
#include "outbound_task.h"
#include "types.h"

namespace xmidictrl {

class device_list;

class device {
public:
    device(std::string_view name, unsigned int port_in, unsigned int port_out, mode_out mode_out, std::shared_ptr<device_list> device_list);
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
    void process_outbound_mappings(std::string_view sl_value);
    void process_outbound_reset();

private:
    void create_outbound_thread();
    void process_outbound_tasks();

    void add_outbound_task(const std::shared_ptr<outbound_task> &task);

    std::shared_ptr<device_list> m_device_list;

    std::string m_name;
    unsigned int m_port_in;
    unsigned int m_port_out;

    mode_out m_mode_out;

    time_point m_time_sent {time_point::min()};

    map_in_list m_map_in;
    map_out_list m_map_out;

    std::unique_ptr<RtMidiIn> m_midi_in;
    std::unique_ptr<RtMidiOut> m_midi_out;

    std::unique_ptr<std::thread> m_outbound_thread;
    std::condition_variable m_new_outbound_task;
    std::atomic<bool> m_exit_outbound_thread {false};
    std::mutex m_outbound_mutex;

    std::queue<std::shared_ptr<outbound_task>> m_outbound_tasks;
    std::set<std::string> m_ch_cc_locked;
};

} // Namespace xmidictrl

#endif // DEVICE_H