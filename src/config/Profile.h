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

#ifndef PROFILE_H
#define PROFILE_H

// Standard
#include <memory>
#include <string>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "Config.h"
#include "DeviceList.h"
#include "Mapping.h"
#include "Types.h"
#include "XPlane.h"

namespace XMidiCtrl {

class Profile : public Config {
public:
    explicit Profile(XPlane::ptr xplane);
    ~Profile() = default;

    typedef std::shared_ptr<Profile> ptr;

    bool load();

    DeviceList::ptr createMidiDevices();

private:
    std::string getProfileFileName();

    void createInboundMappingForDevice(int deviceNo, toml::array settings, Device::ptr device);
    
    static MappingType translateMapTypeStr(std::string_view typeStr);

    Mapping::ptr readSettingsForCommand(int deviceNo, int mapNo, int controlChange, toml::value* settings);
    Mapping::ptr readSettingsForDataref(int deviceNo, int mapNo, int controlChange, toml::value* settings);
    Mapping::ptr readSettingsForSlider(int deviceNo, int mapNo, int controlChange, toml::value* settings);
    Mapping::ptr readSettingsForPushAndPull(int deviceNo, int mapNo, int controlChange, toml::value* settings);
    Mapping::ptr readSettingsForEncoder(int deviceNo, int mapNo, int controlChange, toml::value* settings);
};

} // Namespace XMidiCtrl

#endif // PROFILE_H