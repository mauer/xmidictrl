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
 * Add a midi event
 */
void device_list::add_event(const std::shared_ptr<task> &event) {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    m_inbound_events.push(event);
}


/**
 * Process events
 */
void device_list::process_inbound_events() {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    // process the midi inbound queue for each midi device
    while (!m_inbound_events.empty()) {
        std::shared_ptr<task> event = m_inbound_events.front();

        if (event == nullptr)
            continue;

        // perform the action related to the mapping
        if (event->map != nullptr)
            event->map->execute(*event->msg);

        // delete entry from list
        m_inbound_events.pop();
    }
}


/**
 * Process the midi outbound mappings
 */
void device_list::process_outbound_mappings()
{
    for (auto const &device: m_device_list) {
        if (device != nullptr)
            device->process_outbound_mappings();
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