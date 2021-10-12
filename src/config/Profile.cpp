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

// Standard
#include <filesystem>

// X-Plane SDK
#include "XPLMPlanes.h"

// XMidiCtrl
#include "Mapping.h"
#include "MappingCommand.h"
#include "MappingDataref.h"
#include "MappingEncoder.h"
#include "MappingPushAndPull.h"
#include "MappingSlider.h"
#include "Logger.h"
#include "Profile.h"
#include "Types.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Profile::Profile() : Config() {};


/**
 * Destructor
 */
Profile::~Profile() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Load the settings for the current aircraft
 */
bool Profile::load() {
    LOG_INFO << "PROFILE :: Load aircraft profile" << LOG_END

    // load profile for current aircraft
    return Config::load(determineProfileFileName());
}


/**
 * Create all defined midi devices
 */
void Profile::createMidiDevices(const DeviceList::ptr& deviceList) {
    // make sure we start with an empty list
    deviceList->clear();

    // only continue if the file contains some settings
    if (m_config.type() == toml::value_t::empty)
        return;

    try {
        // get all devices
        auto devices = toml::find<std::vector<toml::table>>(m_config, CFG_KEY_DEVICE);

        LOG_INFO << "PROFILE :: " << devices.size() << " Device(s) found in profile" << LOG_END

        // parse every device
        for (int i = 0; i < static_cast<int>(devices.size()); i++) {
            std::string name;
            unsigned int portIn;
            unsigned int portOut;
            toml::value deviceSettings = devices[i];

            try {
                // name
                if (deviceSettings.contains(CFG_KEY_NAME))
                    name = deviceSettings[CFG_KEY_NAME].as_string();
                else {
                    name = "<undefined>";
                    LOG_WARN << "PROFILE :: Device (" << i << ") :: Parameter '" << CFG_KEY_NAME << "' is missing" << LOG_END
                }

                // port in
                if (deviceSettings.contains(CFG_KEY_PORT_IN))
                    portIn = static_cast<unsigned int>(deviceSettings[CFG_KEY_PORT_IN].as_integer());
                else {
                    LOG_ERROR << "PROFILE :: Device (" << i << ") :: Parameter '" << CFG_KEY_PORT_IN << "' is missing"
                              << LOG_END
                    continue;
                }

                // port out
                if (deviceSettings.contains(CFG_KEY_PORT_OUT))
                    portOut = static_cast<unsigned int>(deviceSettings[CFG_KEY_PORT_OUT].as_integer());
                else {
                    LOG_ERROR << "PROFILE :: Device (" << i << ") :: Parameter '" << CFG_KEY_PORT_OUT << "' is missing"
                              << LOG_END
                    continue;
                }

                // create device and mapping
                std::shared_ptr<Device> device = deviceList->createDevice(name, portIn, portOut);
                if (device)
                    createMappingForDevice(i, deviceSettings["mapping"].as_array(), device);

            } catch (const std::out_of_range& error) {
                LOG_ERROR << "PROFILE :: Error reading config for midi device (" << i << ")" << LOG_END
                LOG_ERROR << error.what() << LOG_END
            }  catch (toml::type_error& error) {
                LOG_ERROR << "PROFILE :: Error reading config for midi device (" << i << ")" << LOG_END
                LOG_ERROR << error.what() << LOG_END
            }
        }
    } catch (std::out_of_range& error) {
        LOG_WARN << "PROFILE :: No midi devices found in config" << LOG_END
        LOG_WARN << error.what() << LOG_END
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Determine the config filename for the currently loaded aircraft
 */
std::string Profile::determineProfileFileName() {
    std::string fileName;

    char aircraftFileName[256];
    char aircraftPath[512];

    // aircraft with index 0 is the user aircraft
    XPLMGetNthAircraftModel(0, aircraftFileName, aircraftPath);

    LOG_INFO << "PROFILE :: Current Aircraft :: Filename = " << aircraftFileName << LOG_END
    LOG_INFO << "PROFILE :: Current Aircraft :: Path = " << aircraftPath << LOG_END

    if (!std::string(aircraftFileName).empty()) {
        // remove filename from path
        fileName = std::string(aircraftPath);
        fileName = fileName.substr(0, fileName.size() - std::string(aircraftFileName).size());
        fileName = fileName + FILENAME_PROFILE;
    }

    LOG_INFO << "PROFILE :: Current Aircraft Profile :: Filename = " << fileName << LOG_END

    return fileName;
}


/**
 * Create the mapping for a device and store it
 */
void Profile::createMappingForDevice(int deviceNo, toml::array settings, const std::shared_ptr<Device>& device) {
    LOG_INFO << "PROFILE :: " << settings.size() << " Mapping(s) found" << LOG_END

    // parse each mapping entry
    for (int i = 0; i < static_cast<int>(settings.size()); i++) {
        int cc = -1;
        MappingType type = MappingType::None;

        std::shared_ptr<Mapping> mapping;

        LOG_INFO << "PROFILE :: Reading mapping " << i << LOG_END

        try {
            if (settings[i].contains(CFG_KEY_CC)) {
                cc = static_cast<int>( settings[i][CFG_KEY_CC].as_integer());
                LOG_INFO << "PROFILE :: Mapping (" << i << ") :: CC = " << cc << LOG_END
            } else
                LOG_ERROR << "PROFILE :: Mapping (" << i << ") :: Parameter '" << CFG_KEY_CC << "' is missing" << LOG_END

            if (settings[i].contains(CFG_KEY_TYPE)) {
                std::string_view typeStr { settings[i][CFG_KEY_TYPE].as_string() };
                LOG_INFO << "PROFILE :: Mapping (" << i << ") :: Type = " << typeStr << LOG_END

                // get the mapping type
                type = translateMapTypeStr(typeStr);
            } else
                LOG_ERROR << "PROFILE :: Mapping (" << i << ") :: Parameter '" << CFG_KEY_TYPE << "' is missing" << LOG_END

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case MappingType::Command:
                    mapping = readSettingsForCommand(cc, &settings[i]);
                    break;

                case MappingType::Dataref:
                    mapping = readSettingsForDataref(cc, &settings[i]);
                    break;

                case MappingType::Slider:
                    mapping = readSettingsForSlider(cc, &settings[i]);
                    break;

                case MappingType::PushAndPull:
                    mapping = readSettingsForPushAndPull(cc, &settings[i]);
                    break;

                case MappingType::Encoder:
                    mapping = readSettingsForEncoder(cc, &settings[i]);
                    break;

                case MappingType::Internal:
                    break;

                case MappingType::None:
                    LOG_ERROR << "PROFILE :: Mapping (" << i << ") :: Invalid mapping type" << LOG_END
                    break;
            }

            if (!mapping) {
                LOG_ERROR << "PROFILE :: Error reading mapping " << i << " for midi device " << deviceNo << LOG_END
                continue;
            }

            if (mapping->check())
                device->addMapping(mapping);
            else
                LOG_ERROR << "PROFILE :: Parameters incomplete for CC " << cc << LOG_END

        } catch (toml::type_error& error) {
            LOG_ERROR << "PROFILE :: Error reading mapping " << i << " for midi device " << deviceNo << LOG_END
            LOG_ERROR << error.what() << LOG_END
        }
    }
}


/**
 * Translate a type string to an enum value
 */
MappingType Profile::translateMapTypeStr(std::string_view typeStr) {
    MappingType mapType = MappingType::None;

    if (typeStr == CFG_MAPTYPE_COMMAND)
        mapType = MappingType::Command;
    else if (typeStr == CFG_MAPTYPE_SLIDER)
        mapType = MappingType::Slider;
    else if (typeStr == CFG_MAPTYPE_DATAREF)
        mapType = MappingType::Dataref;
    else if (typeStr == CFG_MAPTYPE_PUSHANDPULL)
        mapType = MappingType::PushAndPull;
    else if (typeStr == CFG_MAPTYPE_ENCODER)
        mapType = MappingType::Encoder;
    else if (typeStr == CFG_MAPTYPE_INTERNAL)
        mapType = MappingType::Internal;

    return mapType;
}


/**
 * Read settings for mapping type command
 */
Mapping::ptr Profile::readSettingsForCommand(int controlChange, toml::value* settings) {
    LOG_INFO << "PROFILE :: Read settings for type = 'cmd'" << LOG_END

    std::shared_ptr<MappingCommand> mapping = std::make_shared<MappingCommand>(controlChange);

    // read the actual command
    try {
        if (settings->contains(CFG_KEY_COMMAND)) {
            mapping->setCommand(settings->at(CFG_KEY_COMMAND).as_string().str);
            LOG_INFO << "PROFILE :: Command = " << mapping->command() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND << "' not found for CC " << controlChange << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "PROFILE :: Error reading mapping for CC " << controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type dataref
 */
Mapping::ptr Profile::readSettingsForDataref(int controlChange, toml::value* settings) {
    LOG_INFO << "PROFILE :: Read settings for type = 'drf'" << LOG_END

    std::shared_ptr<MappingDataref> mapping = std::make_shared<MappingDataref>(controlChange);

    try {
        // read the actual dataref
        if (settings->contains(CFG_KEY_DATAREF)) {
            mapping->setDataref(settings->at(CFG_KEY_DATAREF).as_string().str);
            LOG_INFO << "PROFILE :: Dataref = " << mapping->dataref() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_DATAREF << "' not found for CC " << controlChange << LOG_END

        // read value on
        if (settings->contains(CFG_KEY_VALUE_ON)) {
            mapping->setValueOn(settings->at(CFG_KEY_VALUE_ON).as_string().str);
            LOG_INFO << "PROFILE :: Value On = " << mapping->valueOn() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_VALUE_ON << "' not found for CC " << controlChange << LOG_END

        // read value off
        if (settings->contains(CFG_KEY_VALUE_OFF)) {
            mapping->setValueOff(settings->at(CFG_KEY_VALUE_OFF).as_string().str);
            LOG_INFO << "PROFILE :: Value Off = " << mapping->valueOff() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_VALUE_OFF << "' not found for CC " << controlChange << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "PROFILE :: Error reading mapping for CC " << controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type slider
 */
Mapping::ptr Profile::readSettingsForSlider(int controlChange, toml::value* settings) {
    LOG_INFO << "PROFILE :: Read settings for type = 'sld'" << LOG_END

    std::shared_ptr<MappingSlider> mapping = std::make_shared<MappingSlider>(controlChange);

    // read the four commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_TOP)) {
            mapping->setCommandTop(settings->at(CFG_KEY_COMMAND_TOP).as_string().str);
            LOG_INFO << "PROFILE :: Command Top = " << mapping->commandTop() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND_TOP << "' not found for CC " << controlChange << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_BOTTOM)) {
            mapping->setCommandBottom(settings->at(CFG_KEY_COMMAND_BOTTOM).as_string().str);
            LOG_INFO << "PROFILE :: Command Bottom = " << mapping->commandBottom() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND_BOTTOM << "' not found for CC " << controlChange << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "PROFILE :: Error reading mapping for CC " << controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type push and pull
 */
Mapping::ptr Profile::readSettingsForPushAndPull(int controlChange, toml::value* settings) {
    LOG_INFO << "PROFILE :: Read settings for type = 'pnp'" << LOG_END

    std::shared_ptr<MappingPushAndPull> mapping = std::make_shared<MappingPushAndPull>(controlChange);

    // read the two commands for push and pull
    try {
        if (settings->contains(CFG_KEY_COMMAND_PUSH)) {
            mapping->setCommandPush(settings->at(CFG_KEY_COMMAND_PUSH).as_string().str);
            LOG_INFO << "PROFILE :: Command Push = " << mapping->commandPush() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND_PUSH << "' not found for CC " << controlChange << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_PULL)) {
            mapping->setCommandPull(settings->at(CFG_KEY_COMMAND_PULL).as_string().str);
            LOG_INFO << "PROFILE :: Command Pull = " << mapping->commandPull() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND_PULL << "' not found for CC " << controlChange << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "PROFILE :: Error reading mapping for CC " << controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type encoder
 */
Mapping::ptr Profile::readSettingsForEncoder(int controlChange, toml::value* settings) {
    LOG_INFO << "PROFILE :: Read settings for type = 'enc'" << LOG_END

    std::shared_ptr<MappingEncoder> mapping = std::make_shared<MappingEncoder>(controlChange);

    // read the four commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_UP)) {
            mapping->setCommandUp(settings->at(CFG_KEY_COMMAND_UP).as_string().str);
            LOG_INFO << "PROFILE :: Command Up = " << mapping->commandUp() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND_UP << "' not found for CC " << controlChange << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_DOWN)) {
            mapping->setCommandDown(settings->at(CFG_KEY_COMMAND_DOWN).as_string().str);
            LOG_INFO << "PROFILE :: Command Down = " << mapping->commandDown() << LOG_END
        } else
            LOG_ERROR << "PROFILE :: Parameter '" << CFG_KEY_COMMAND_DOWN << "' not found for CC " << controlChange << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_FAST_UP)) {
            mapping->setCommandFastUp(settings->at(CFG_KEY_COMMAND_FAST_UP).as_string().str);
            LOG_INFO << "PROFILE :: Command Up (fast) = " << mapping->commandFastUp() << LOG_END
        }

        if (settings->contains(CFG_KEY_COMMAND_FAST_DOWN)) {
            mapping->setCommandFastDown(settings->at(CFG_KEY_COMMAND_FAST_DOWN).as_string().str);
            LOG_INFO << "PROFILE :: Command Down (fast) = " << mapping->commandFastDown() << LOG_END
        }

    } catch (toml::type_error& error) {
        LOG_ERROR << "PROFILE :: Error reading mapping for CC " << controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}

} // Namespace XMidiCtrl