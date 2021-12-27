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

// XMidiCtrl
#include "logger.h"

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
std::shared_ptr<device> device_list::create_device(std::string_view name, unsigned int port_in, unsigned int port_out)
{
    std::shared_ptr<device> dev = std::make_shared<device>(name, port_in, port_out, shared_from_this());
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
 * Add a task to be executed
 */
void device_list::add_task(const std::shared_ptr<task> &t)
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    LOG_DEBUG << " --> Task added to queue" << LOG_END

    m_inbound_tasks.push(t);
}


/**
 * Process inbound events
 */
void device_list::process_inbound_events(std::string_view sl_value)
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    std::queue<std::shared_ptr<task>> temp_list;

    // process the midi inbound queue for each midi device
    while (!m_inbound_tasks.empty()) {
        std::shared_ptr<task> t = m_inbound_tasks.front();

        if (t == nullptr || t->map == nullptr)
            continue;

        // perform the action related to the mapping
        if (!t->map->execute(*t->msg, sl_value)) {
            // store in temp list
            temp_list.push(t);
        }

        // delete entry from list
        m_inbound_tasks.pop();
    }

    // add temp tasks to queue again, will be executed in next flight loop
    while (!temp_list.empty()) {
        std::shared_ptr<task> t = temp_list.front();
        m_inbound_tasks.push(t);

        temp_list.pop();
    }
}


/**
 * Process the midi outbound mappings
 */
void device_list::process_outbound_mappings(std::string_view sl_value)
{
    for (auto const &device: m_device_list) {
        if (device != nullptr)
            device->process_outbound_mappings(sl_value);
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