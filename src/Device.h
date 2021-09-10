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

#ifndef DEVICE_H
#define DEVICE_H

// Standard
#include <chrono>
#include <map>
#include <memory>

// X-Plane SDK
#include "XPLMUtilities.h"

// RtMidi
#include "RtMidi.h"

// XMidiCtrl
#include "Global.h"
#include "Types.h"

namespace XMidiCtrl {

// let's make life a little easier
using time_point = std::chrono::time_point<std::chrono::system_clock>;


class Device {
public:
    explicit Device(DeviceSettings settings);
    ~Device();

    // no copying or copy assignments are allowed
    Device(Device const &) = delete;
    Device &operator=(Device const &) = delete;

    bool openConnections();
    void closeConnections();

    static void midiCallback(double deltatime, std::vector<unsigned char>* message, void* userdata);
    void processMessage(double deltatime, std::vector<unsigned char>* message);
    
    std::string_view name() const;

private:
    void saveEventDateTime(int controlChange);
    double retrieveEventDateTime(int controlChange);

    DeviceSettings m_settings;

    std::unique_ptr<RtMidiIn>  m_midiIn;
    std::unique_ptr<RtMidiOut> m_midiOut;

    std::map<int, time_point> m_eventStorage;
};

} // Namespace XMidiCtrl

#endif // DEVICE_H