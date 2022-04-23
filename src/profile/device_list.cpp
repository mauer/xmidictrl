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

#include "device_list.h"

// Standard
#include <mutex>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
device_list::~device_list()
{
    clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create a new midi device
 */
std::shared_ptr<device> device_list::create_device(text_logger &in_text_log,
                                                   midi_logger &in_midi_log,
                                                   std::string_view in_name,
                                                   unsigned int in_port_in,
                                                   unsigned int in_port_out,
                                                   mode_out in_mode_out)
{
    std::shared_ptr<device>
        dev = std::make_shared<device>(in_text_log, in_midi_log, in_name, in_port_in, in_port_out, in_mode_out);
    m_device_list.push_back(dev);

    return dev;
}


/**
 * Open all midi connections
 */
bool device_list::open_connections()
{
    bool result = true;

    for (auto const &device: m_device_list) {
        if (device != nullptr) {
            if (!device->open_connections())
                result = false;
        }
    }

    return result;
}


/**
 * Close all midi connections
 */
void device_list::close_connections()
{
    for (auto const &device: m_device_list) {
        if (device != nullptr) {
            device->close_connections();
        }
    }
}


/**
 * Process the midi init mappings
 */
void device_list::process_init_mappings(text_logger &in_log)
{
    for (auto const &device: m_device_list) {
        if (device != nullptr)
            device->process_init_mappings(in_log);
    }
}


/**
 * Process the midi outbound mappings
 */
void device_list::process_outbound_mappings(text_logger &in_log)
{
    for (auto const &device: m_device_list) {
        if (device != nullptr)
            device->process_outbound_mappings(in_log);
    }
}


/**
 * Process an outbound reset of all MIDI devices
 */
void device_list::process_outbound_reset()
{
    for (auto const &device: m_device_list) {
        if (device != nullptr)
            device->process_outbound_reset();
    }
}


/**
 * Clear the device list
 */
void device_list::clear()
{
    close_connections();

    m_device_list.clear();
}


/**
 * Return the number of devices
 */
unsigned int device_list::size()
{
    return m_device_list.size();
}

} // Namespace xmidictrl