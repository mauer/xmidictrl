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

#ifndef XMC_DEVICE_LIST_H
#define XMC_DEVICE_LIST_H

// Standard
#include <memory>
#include <vector>
#include <queue>

// XMidiCtrl
#include "device.h"
#include "device_settings.h"
#include "inbound_task.h"
#include "midi_device.h"
#include "midi_logger.h"
#include "text_logger.h"
#include "virtual_device.h"

namespace xmidictrl {

class device_list {
public:
    explicit device_list() = default;
    ~device_list();

    std::shared_ptr<midi_device> create_midi_device(text_logger& in_text_log,
                                                    midi_logger& in_midi_log,
                                                    const std::shared_ptr<device_settings>& in_settings);

    std::shared_ptr<virtual_device> create_virtual_device(text_logger& in_text_log,
                                                          midi_logger& in_midi_log,
                                                          const std::shared_ptr<device_settings>& in_settings);

    bool open_connections();
    void close_connections();

    std::shared_ptr<virtual_device> find_virtual_device();

    void process_init_mappings();
    void process_outbound_mappings(text_logger& in_log);
    void process_outbound_reset();

    void clear();
    size_t size();

private:
    std::vector<std::shared_ptr<device>> m_device_list {};
};

} // Namespace xmidictrl

#endif // XMC_DEVICE_LIST_H
