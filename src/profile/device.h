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
#include <text_logger.h>
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
#include "map_init.h"
#include "map_init_list.h"
#include "map_out.h"
#include "map_out_list.h"
#include "midi_logger.h"
#include "outbound_task.h"
#include "types.h"

namespace xmidictrl {

class device_list;

class device {
public:
    device(text_logger &in_text_log,
           midi_logger &in_midi_log,
           std::string_view in_name,
           unsigned int in_port_in,
           unsigned int in_port_out,
           mode_out in_mode_out);
    ~device();

    // no copying or copy assignments are allowed
    device(device const &) = delete;
    device &operator=(device const &) = delete;

    void add_init_map(std::shared_ptr<map_init> &in_mapping);
    void add_inbound_map(std::shared_ptr<map_in> &in_mapping);
    void add_outbound_map(std::shared_ptr<map_out> &in_mapping);

    bool open_connections();
    void close_connections();

    static void midi_callback([[maybe_unused]] double in_deltatime, std::vector<unsigned char> *in_message, void *in_userdata);

    void process_init_mappings(text_logger &in_log);

    void process_inbound_message(std::vector<unsigned char> *in_message);

    void process_outbound_mappings(text_logger &in_log);
    void process_outbound_reset();

private:
    void create_outbound_thread();
    void process_outbound_tasks();

    void add_outbound_task(const std::shared_ptr<outbound_task> &in_task);

    text_logger &m_text_log;
    midi_logger &m_midi_log;

    std::string m_name;
    unsigned int m_port_in;
    unsigned int m_port_out;

    mode_out m_mode_out;

    time_point m_time_sent {time_point::min()};

    map_init_list m_map_init;
    map_in_list m_map_in;
    map_out_list m_map_out;

    std::unique_ptr<RtMidiIn> m_midi_in;
    std::unique_ptr<RtMidiOut> m_midi_out;

    std::unique_ptr<std::thread> m_outbound_thread;
    std::condition_variable m_new_outbound_task;
    std::atomic<bool> m_exit_outbound_thread {false};
    std::mutex m_outbound_mutex;

    std::queue<std::shared_ptr<midi_message>> m_outbound_msg;
    std::set<std::string> m_outbound_locked;
};

} // Namespace xmidictrl

#endif // DEVICE_H