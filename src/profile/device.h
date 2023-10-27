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

#ifndef XMC_DEVICE_H
#define XMC_DEVICE_H

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

// XMidiCtrl
#include "device_settings.h"
#include "map_in.h"
#include "map_in_list.h"
#include "midi_logger.h"
#include "types.h"

namespace xmidictrl {

class device_list;

class device {
public:
    device(text_logger& in_text_log, midi_logger& in_midi_log, std::shared_ptr<device_settings> in_settings);
    virtual ~device() = default;

    // no copying or copy assignments are allowed
    device(device const&) = delete;
    device& operator=(device const&) = delete;

    virtual device_type type() = 0;

    device_settings& settings();

    std::string sl_value() const;
    void set_sl_value(std::string_view in_sl_value);

    void add_inbound_map(std::shared_ptr<map_in>& in_mapping);

protected:
    text_logger& text_log();
    midi_logger& midi_log();

    map_in_list& mapping_in();

private:
    text_logger& m_text_log;
    midi_logger& m_midi_log;

    std::shared_ptr<device_settings> m_settings;

    std::string m_sl_value;

    map_in_list m_map_in;
};

} // Namespace xmidictrl

#endif // XMC_DEVICE_H
