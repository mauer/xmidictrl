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
#include <queue>
#include <vector>

// XMidiCtrl
#include "device.h"
#include "device_settings.h"
#include "environment.h"
#include "midi_device.h"
#include "midi_logger.h"
#include "text_logger.h"
#include "virtual_device.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class device_list {
public:
    explicit device_list(environment& in_env);
    ~device_list();

    device* create_midi_device(text_logger& in_text_log,
                               midi_logger& in_midi_log,
                               std::unique_ptr<device_settings> in_settings);

    device* create_virtual_device(text_logger& in_text_log,
                                  midi_logger& in_midi_log,
                                  std::unique_ptr<device_settings> in_settings);

    std::vector<std::unique_ptr<device>>::iterator begin();
    std::vector<std::unique_ptr<device>>::iterator end();

    bool open_connections();
    void close_connections();

    virtual_device* find_virtual_device();

    void update_sl_values(text_logger& in_log, environment& in_env);

    void process_init_mappings();
    void process_outbound_mappings(text_logger& in_log);
    void process_outbound_reset();

    void clear();
    size_t size();

private:
    environment& m_env;

    std::vector<std::unique_ptr<device>> m_device_list {};
};

} // Namespace xmidictrl

#endif // XMC_DEVICE_LIST_H