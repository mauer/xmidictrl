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

// XMidiCtrl
#include "DeviceList.h"
#include "Logger.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
DeviceList::DeviceList() = default;


/**
 * Destructor
 */
DeviceList::~DeviceList() {
    clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create a new midi device
 */
Device::ptr DeviceList::createDevice(std::string_view name, unsigned int portIn, unsigned int portOut) {
    Device::ptr device = std::make_shared<Device>(name, portIn, portOut);
    m_list.push_back(device);

    return device;
}


/**
 * Open all midi connections
 */
void DeviceList::openConnections() {
    for (auto const &device: m_list) {
        if (device != nullptr) {
            device->openConnections();
        }
    }
}


/**
 * Close all midi connections
 */
void DeviceList::closeConnections() {
    for (auto const &device: m_list) {
        if (device != nullptr) {
            device->closeConnections();
        }
    }
}


/**
 * Clear the device list
 */
void DeviceList::clear() {
    closeConnections();

    m_list.clear();
}


/**
 * Return the number of devices
 */
unsigned int DeviceList::size() {
    return m_list.size();
}

} // Namespace XMidiCtrl