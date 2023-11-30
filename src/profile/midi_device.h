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

#ifndef XMC_MIDI_DEVICE_H
#define XMC_MIDI_DEVICE_H

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
#include "device.h"
#include "device_settings.h"
#include "map_in.h"
#include "map_in_list.h"
#include "map_init.h"
#include "map_init_list.h"
#include "map_out.h"
#include "map_out_list.h"
#include "midi_logger.h"
#include "outbound_task.h"
#include "types.h"

#ifdef min
    #undef min // prevent clash with time_point::min() later
#endif

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class midi_device : public device {
public:
    midi_device(text_logger& in_text_log, midi_logger& in_midi_log, std::unique_ptr<device_settings> in_settings);
    ~midi_device() override;

    // no copying or copy assignments are allowed
    midi_device(midi_device const&) = delete;
    midi_device& operator=(midi_device const&) = delete;

    device_type type() override;

    map_init_list& mapping_init();
    map_out_list& mapping_out();

    //void add_init_map(std::shared_ptr<map_init>& in_mapping);
    //void add_outbound_map(std::shared_ptr<map_out>& in_mapping);

    bool open_connections();
    void close_connections();

    static void midi_callback([[maybe_unused]] double in_deltatime,
                              std::vector<unsigned char>* in_message,
                              void* in_userdata);

    void process_init_mappings();

    void process_inbound_message(std::vector<unsigned char>* in_message);

    void process_outbound_mappings(text_logger& in_log);
    void process_outbound_reset();

private:
    void create_outbound_thread();
    void process_outbound_tasks();

    void add_outbound_task(const std::shared_ptr<outbound_task>& in_task);

    time_point m_time_sent {time_point::min()};

    std::unique_ptr<map_init_list> m_map_init;
    std::unique_ptr<map_out_list> m_map_out;

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

#endif // XMC_MIDI_DEVICE_H
