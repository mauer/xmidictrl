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

#ifndef TYPES_H
#define TYPES_H

// Standard
#include <map>
#include <string>

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTANTS
//---------------------------------------------------------------------------------------------------------------------

// Interval of flight loop
#define XMIDICTRL_FLIGHTLOOP_INTERVAL (-1.0)




//---------------------------------------------------------------------------------------------------------------------
//   ENUMERATIONS
//---------------------------------------------------------------------------------------------------------------------

// Midi mapping types
enum class MappingType {
    None,
    Command,
    DataRef,
    Slider,
    PushAndPull,
    Encoder,
    Internal
};




//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

// List off connected midi devices
class Device;
typedef std::map<std::string, std::shared_ptr<Device>> MidiDeviceList;




//---------------------------------------------------------------------------------------------------------------------
//   STRUCTURES
//---------------------------------------------------------------------------------------------------------------------

// Midi mapping
struct MidiMapping {
    int controlChange;
    MappingType type;

    std::string command;

    std::string commandPush;
    std::string commandPull;

    std::string commandUp;
    std::string commandDown;

    std::string commandUpFast;
    std::string commandDownFast;

    std::string dataRef;
    std::string valueOn;
    std::string valueOff;
};


// Midi device settings
struct DeviceSettings {
    std::string name;

    int portIn;
    int portOut;

    std::map<int, MidiMapping> mapping;
};


// Single midi event
struct MidiEvent {
    int status;
    int controlChange;
    int velocity;

    MidiMapping mapping;
};

} // Namespace XMidiCtrl

#endif // TYPES_H
