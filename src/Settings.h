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

#ifndef SETTINGS_H
#define SETTINGS_H

// Standard
#include <string>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "Types.h"

namespace XMidiCtrl {

// Keys for the config file
const char* const CFG_KEY_CC                = "CC";
const char* const CFG_KEY_COMMAND           = "command";
const char* const CFG_KEY_COMMAND_DOWN      = "command_down";
const char* const CFG_KEY_COMMAND_DOWN_FAST = "command_down_fast";
const char* const CFG_KEY_COMMAND_PULL      = "command_pull";
const char* const CFG_KEY_COMMAND_PUSH      = "command_push";
const char* const CFG_KEY_COMMAND_UP        = "command_up";
const char* const CFG_KEY_COMMAND_UP_FAST   = "command_up_fast";
const char* const CFG_KEY_DATAREF           = "dataref";
const char* const CFG_KEY_DEVICE            = "device";
const char* const CFG_KEY_NAME              = "name";
const char* const CFG_KEY_PORT_IN           = "port_in";
const char* const CFG_KEY_PORT_OUT          = "port_out";
const char* const CFG_KEY_TYPE              = "type";
const char* const CFG_KEY_VALUE_ON          = "value_on";
const char* const CFG_KEY_VALUE_OFF         = "value_off";
const char* const CFG_KEY_VERSION           = "version";

class Settings {
public:
    Settings();
    ~Settings();

    void reloadSettingsForAircraft();

    std::vector<DeviceSettings> deviceList();

private:
    std::string getAircraftConfigFileName();

    std::map<int, MidiMapping> mappingForDevice(toml::array settings); 
    
    MappingType translateMapTypeStr(std::string_view typeStr);
    
    bool readSettingsForCommand(MidiMapping* mapping, toml::value* settings);
    bool readSettingsForDataRef(MidiMapping* mapping, toml::value* settings);
    bool readSettingsForSlider(MidiMapping* mapping, toml::value* settings);
    bool readSettingsForPushAndPull(MidiMapping* mapping, toml::value* settings);
    bool readSettingsForEncoder(MidiMapping* mapping, toml::value* settings);

    toml::value m_aircraftConfig;
    toml::value m_generalConfig;
};

} // Namespace XMidiCtrl

#endif // SETTINGS_H