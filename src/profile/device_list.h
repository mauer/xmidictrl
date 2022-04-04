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

#ifndef DEVICE_LIST_H
#define DEVICE_LIST_H

// Standard
#include <memory>
#include <vector>
#include <queue>

// XMidiCtrl
#include "device.h"
#include "inbound_task.h"
#include "text_logger.h"
#include "midi_logger.h"

namespace xmidictrl {

class device_list {
public:
    explicit device_list() = default;
    ~device_list();

    std::shared_ptr<device> create_device(text_logger &in_text_log,
                                          midi_logger &in_midi_log,
                                          std::string_view in_name,
                                          unsigned int in_port_in,
                                          unsigned int in_port_out,
                                          mode_out in_mode_out);

    bool open_connections();
    void close_connections();

    void process_outbound_mappings(text_logger &in_log);
    void process_outbound_reset();

    void clear();
    unsigned int size();

private:
    std::vector<std::shared_ptr<device>> m_device_list {};
};

} // Namespace xmidictrl

#endif // DEVICE_LIST_H