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
std::shared_ptr<midi_device> device_list::create_midi_device(text_logger& in_text_log,
                                                             midi_logger& in_midi_log,
                                                             const std::shared_ptr<device_settings>& in_settings)
{
    auto dev = std::make_shared<midi_device>(in_text_log, in_midi_log, in_settings);
    m_device_list.push_back(dev);

    return dev;
}


/**
 * Create a new virtual device
 */
std::shared_ptr<virtual_device> device_list::create_virtual_device(text_logger& in_text_log,
                                                                   midi_logger& in_midi_log,
                                                                   const std::shared_ptr<device_settings>& in_settings)
{
    auto dev = std::make_shared<virtual_device>(in_text_log, in_midi_log, in_settings);
    m_device_list.push_back(dev);

    return dev;
}


/**
 * Return the begin iterator
 */
std::vector<std::shared_ptr<device>>::iterator device_list::begin()
{
    return m_device_list.begin();
}


/**
 * Return the end iterator
 */
std::vector<std::shared_ptr<device>>::iterator device_list::end()
{
    return m_device_list.end();
}


/**
 * Open all midi connections
 */
bool device_list::open_connections()
{
    bool result = true;

    for (auto const& device: m_device_list) {
        if (device != nullptr && device->type() == device_type::midi_device) {
            auto midi_dev = std::static_pointer_cast<midi_device>(device);

            if (!midi_dev->open_connections())
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
    for (auto const& device: m_device_list) {
        if (device != nullptr && device->type() == device_type::midi_device) {
            auto midi_dev = std::static_pointer_cast<midi_device>(device);

            midi_dev->close_connections();
        }
    }
}


/**
 * Find and return the virtual device (if available, otherwise a nullptr)
 */
std::shared_ptr<virtual_device> device_list::find_virtual_device()
{
    std::shared_ptr<virtual_device> virtual_dev {nullptr};

    for (auto const& device: m_device_list) {
        if (device != nullptr && device->type() == device_type::virtual_device)
            virtual_dev = std::static_pointer_cast<virtual_device>(device);
    }

    return virtual_dev;
}


/**
 * Update the sublayer datarefs in all devices
 */
void device_list::update_sl_values(text_logger& in_log, environment& in_env)
{
    for (auto const& device: m_device_list) {
        if (device != nullptr && !device->settings().sl_dataref.empty()) {
            std::string sl_value {};
            in_env.drf().read(in_log, device->settings().sl_dataref, sl_value);

            device->set_sl_value(sl_value);
        }
    }
}


/**
 * Process the midi init mappings
 */
void device_list::process_init_mappings()
{
    for (auto const& device: m_device_list) {
        if (device != nullptr && device->type() == device_type::midi_device) {
            auto midi_dev = std::static_pointer_cast<midi_device>(device);

            midi_dev->process_init_mappings();
        }
    }
}


/**
 * Process the midi outbound mappings
 */
void device_list::process_outbound_mappings(text_logger& in_log)
{
    for (auto const& device: m_device_list) {
        if (device != nullptr && device->type() == device_type::midi_device) {
            auto midi_dev = std::static_pointer_cast<midi_device>(device);

            midi_dev->process_outbound_mappings(in_log);
        }
    }
}


/**
 * Process an outbound reset of all MIDI devices
 */
void device_list::process_outbound_reset()
{
    for (auto const& device: m_device_list) {
        if (device != nullptr && device->type() == device_type::midi_device) {
            auto midi_dev = std::static_pointer_cast<midi_device>(device);

            midi_dev->process_outbound_reset();
        }
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
size_t device_list::size()
{
    return m_device_list.size();
}

} // Namespace xmidictrl
