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

// XMidiCtrl
#include "Logger.h"
#include "MappingCommand.h"
#include "MappingDataref.h"
#include "MappingEncoder.h"
#include "MappingPushAndPull.h"
#include "MappingSlider.h"
#include "Profile.h"
#include "Types.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Profile::Profile(XPlane::ptr xplane)
        : Config(xplane, "Aircraft Profile") {};




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Load the settings for the current aircraft
 */
bool Profile::load() {
    std::string fileName = getProfileFileName();

    if (!fileName.empty()) {
        LOG_INFO << "Aircraft Profile '" << fileName << "' found" << LOG_END
        return Config::load(fileName);
    } else {
        LOG_INFO << "No profile found for current aircraft" << LOG_END
        return false;
    }
}


/**
 * Create all defined midi devices
 */
bool Profile::createMidiDevices(DeviceList::ptr devices) {
    bool result = true;

    // close all open connections
    devices->clear();

    // only continue if the file contains some settings
    if (m_config.type() == toml::value_t::empty)
        return false;

    try {
        // get all devices
        auto profileDevices = toml::find<std::vector<toml::table>>(m_config, CFG_KEY_DEVICE);

        LOG_INFO << profileDevices.size() << " Device(s) found in aircraft profile" << LOG_END

        // parse every device
        for (int i = 0; i < static_cast<int>(profileDevices.size()); i++) {
            std::string name;
            unsigned int portIn;
            unsigned int portOut;
            toml::value deviceSettings = profileDevices[i];

            try {
                // name
                if (deviceSettings.contains(CFG_KEY_NAME))
                    name = deviceSettings[CFG_KEY_NAME].as_string();
                else {
                    name = "<undefined>";
                    LOG_WARN << "Device (" << i << ") :: Parameter '" << CFG_KEY_NAME << "' is missing" << LOG_END
                }

                // port in
                if (deviceSettings.contains(CFG_KEY_PORT_IN))
                    portIn = static_cast<unsigned int>(deviceSettings[CFG_KEY_PORT_IN].as_integer());
                else {
                    LOG_ERROR << "Device (" << i << ") :: Parameter '" << CFG_KEY_PORT_IN << "' is missing" << LOG_END
                    result = false;
                    continue;
                }

                // port out
                if (deviceSettings.contains(CFG_KEY_PORT_OUT))
                    portOut = static_cast<unsigned int>(deviceSettings[CFG_KEY_PORT_OUT].as_integer());
                else {
                    LOG_ERROR << "Device (" << i << ") :: Parameter '" << CFG_KEY_PORT_OUT << "' is missing" << LOG_END
                    result = false;
                    continue;
                }

                // create device and mapping
                std::shared_ptr<Device> device = devices->createDevice(name, portIn, portOut);
                if (device != nullptr) {
                    // Inbound mapping
                    if (deviceSettings.contains(CFG_KEY_MIDI_IN)) {
                        if (!createInboundMappingForDevice(i, deviceSettings[CFG_KEY_MIDI_IN].as_array(), device))
                            result = false;
                    } else if (deviceSettings.contains(CFG_KEY_MAPPING)) {
                        LOG_WARN << "Device (" << i << ") :: Key '" << CFG_KEY_MAPPING
                                 << "' is deprecated, please rename it to '" << CFG_KEY_MIDI_IN << "'" << LOG_END
                        if (!createInboundMappingForDevice(i, deviceSettings[CFG_KEY_MAPPING].as_array(), device))
                            result = false;
                    } else {
                        LOG_INFO << "Device (" << i << ") :: No midi inbound mapping found" << LOG_END
                    }

                    // TODO Outbound mapping
                }
            } catch (const std::out_of_range& error) {
                LOG_ERROR << "Device (" << i << ") :: Error reading profile" << LOG_END
                LOG_ERROR << error.what() << LOG_END
                result = false;
            }  catch (toml::type_error& error) {
                LOG_ERROR << "Device (" << i << ") :: Error reading profile" << LOG_END
                LOG_ERROR << error.what() << LOG_END
                result = false;
            }
        }
    } catch (std::out_of_range& error) {
        LOG_WARN << "No midi devices found in aircraft profile" << LOG_END
        LOG_WARN << error.what() << LOG_END
    }

    return result;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the profile filename for the current aircraft
 */
std::string Profile::getProfileFileName() {
    std::string fileName;

    // 1. check if there is a profile in the aircraft directory
    fileName = m_xplane->currentAircraftPath().data() + std::string(FILENAME_PROFILE);

    LOG_DEBUG << "Search for aircraft profile '" << fileName << "'" << LOG_END
    if (std::filesystem::exists(fileName)) {
        return fileName;
    }

    // 2. check if there is a profile in the profiles directory including the author and ICAO
    fileName = m_xplane->profilesPath().data() + m_xplane->currentAircraftAuthor() + "_" +
               m_xplane->currentAircraftICAO() + "_" + std::string(FILENAME_PROFILE);

    LOG_DEBUG << "Search for aircraft profile '" << fileName << "'" << LOG_END
    if (std::filesystem::exists(fileName)) {
        return fileName;
    }

    // 2. check if there is a profile in the profiles directory including the ICAO
    fileName = m_xplane->profilesPath().data() + m_xplane->currentAircraftICAO() + "_" + std::string(FILENAME_PROFILE);

    LOG_DEBUG << "Search for aircraft profile '" << fileName << "'" << LOG_END
    if (std::filesystem::exists(fileName)) {
        return fileName;
    }

    // 3. check for the default profile
    fileName = m_xplane->profilesPath().data() + std::string(FILENAME_PROFILE);
    LOG_DEBUG << "Search for aircraft profile '" << fileName << "'" << LOG_END
    if (std::filesystem::exists(fileName)) {
        return fileName;
    }

    return std::string();
}


/**
 * Create the inbound mapping for a device and store it
 */
bool Profile::createInboundMappingForDevice(int deviceNo, toml::array settings, Device::ptr device) {
    bool result = true;

    LOG_INFO << "Device (" << deviceNo << ") :: " << settings.size() << " Mapping(s) found" << LOG_END

    // parse each mapping entry
    for (int i = 0; i < static_cast<int>(settings.size()); i++) {
        int cc = -1;
        MappingType type = MappingType::None;

        std::shared_ptr<Mapping> mapping;

        LOG_DEBUG << "Device (" << deviceNo << ") :: Read mapping(" << i << LOG_END

        try {
            if (settings[i].contains(CFG_KEY_CC)) {
                cc = static_cast<int>( settings[i][CFG_KEY_CC].as_integer());
                LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: cc = '" << cc << "'" << LOG_END
            } else if (settings[i].contains(CFG_KEY_CC_DEPRECATED)) {
                cc = static_cast<int>( settings[i][CFG_KEY_CC_DEPRECATED].as_integer());
                LOG_WARN << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Parameter 'CC' is deprecated, "
                         << "please rename it to 'cc'" << LOG_END
                LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: CC = '" << cc << "'" << LOG_END
            } else {
                LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Parameter '" << CFG_KEY_CC
                          << "' is missing" << LOG_END
                result = false;
            }

            if (settings[i].contains(CFG_KEY_TYPE)) {
                std::string_view typeStr { settings[i][CFG_KEY_TYPE].as_string() };
                LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: type = '" << typeStr << "'"
                          << LOG_END

                // get the mapping type
                type = translateMapTypeStr(typeStr);
            } else {
                LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Parameter '" << CFG_KEY_TYPE
                          << "' is missing" << LOG_END
                result = false;
            }

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case MappingType::Command:
                    mapping = readSettingsForCommand(deviceNo, i, cc, &settings[i]);
                    break;

                case MappingType::Dataref:
                    mapping = readSettingsForDataref(deviceNo, i, cc, &settings[i]);
                    break;

                case MappingType::Slider:
                    mapping = readSettingsForSlider(deviceNo, i, cc, &settings[i]);
                    break;

                case MappingType::PushAndPull:
                    mapping = readSettingsForPushAndPull(deviceNo, i, cc, &settings[i]);
                    break;

                case MappingType::Encoder:
                    mapping = readSettingsForEncoder(deviceNo, i, cc, &settings[i]);
                    break;

                case MappingType::Internal:
                    LOG_WARN << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: "
                             << "Mapping type 'int' is currently unsupported" << LOG_END
                    break;

                case MappingType::None:
                    LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Invalid mapping type"
                              << LOG_END
                    result = false;
                    break;
            }

            if (!mapping) {
                LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Error reading mapping" << LOG_END
                result = false;
                continue;
            }

            if (mapping->check())
                device->addMapping(mapping);
            else {
                LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Parameters incomplete" << LOG_END
                result = false;
            }

        } catch (toml::type_error& error) {
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << i << ") :: Error reading mapping" << LOG_END
            LOG_ERROR << error.what() << LOG_END
            result = false;
        }
    }

    return result;
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
Mapping::ptr Profile::readSettingsForCommand(int deviceNo, int mapNo, int controlChange, toml::value* settings) {
    LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Read settings for type 'cmd'" << LOG_END

    std::shared_ptr<MappingCommand> mapping = std::make_shared<MappingCommand>(m_xplane, controlChange);

    // read the actual command
    try {
        if (settings->contains(CFG_KEY_COMMAND)) {
            mapping->setCommand(settings->at(CFG_KEY_COMMAND).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND << " = '"
                      << mapping->command() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '" << CFG_KEY_COMMAND
                      << "' not found" << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type dataref
 */
Mapping::ptr Profile::readSettingsForDataref(int deviceNo, int mapNo, int controlChange, toml::value* settings) {
    LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Read settings for type 'drf'" << LOG_END

    std::shared_ptr<MappingDataref> mapping = std::make_shared<MappingDataref>(m_xplane, controlChange);

    try {
        // read the actual dataref
        if (settings->contains(CFG_KEY_DATAREF)) {
            mapping->setDataref(settings->at(CFG_KEY_DATAREF).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_DATAREF
                      << " = '" << mapping->dataref() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '" << CFG_KEY_DATAREF
                      << "' not found" << LOG_END

        // read value on
        if (settings->contains(CFG_KEY_VALUE_ON)) {
            mapping->setValueOn(settings->at(CFG_KEY_VALUE_ON).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_VALUE_ON << " = '"
                      << mapping->valueOn() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '" << CFG_KEY_VALUE_ON
                      << "' not found" << LOG_END

        // read value off
        if (settings->contains(CFG_KEY_VALUE_OFF)) {
            mapping->setValueOff(settings->at(CFG_KEY_VALUE_OFF).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_VALUE_OFF << " = '"
                      << mapping->valueOff() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '" << CFG_KEY_VALUE_OFF
                      << "' not found" << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type slider
 */
Mapping::ptr Profile::readSettingsForSlider(int deviceNo, int mapNo, int controlChange, toml::value* settings) {
    LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Read settings for type 'sld'" << LOG_END

    std::shared_ptr<MappingSlider> mapping = std::make_shared<MappingSlider>(m_xplane, controlChange);

    // read the four commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_UP)) {
            mapping->setCommandUp(settings->at(CFG_KEY_COMMAND_UP).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_UP << " = '"
                      << mapping->commandUp() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '" << CFG_KEY_COMMAND_UP
                      << "' not found" << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_MIDDLE)) {
            mapping->setCommandMiddle(settings->at(CFG_KEY_COMMAND_MIDDLE).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_MIDDLE
                      << " = '" << mapping->commandMiddle() << "'" << LOG_END
        }

        if (settings->contains(CFG_KEY_COMMAND_DOWN)) {
            mapping->setCommandDown(settings->at(CFG_KEY_COMMAND_DOWN).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_DOWN
                      << " = '" << mapping->commandDown() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '"
                      << CFG_KEY_COMMAND_DOWN << "' not found" << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type push and pull
 */
Mapping::ptr Profile::readSettingsForPushAndPull(int deviceNo, int mapNo, int controlChange, toml::value* settings) {
    LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Read settings for type 'pnp'" << LOG_END

    std::shared_ptr<MappingPushAndPull> mapping = std::make_shared<MappingPushAndPull>(m_xplane, controlChange);

    // read the two commands for push and pull
    try {
        if (settings->contains(CFG_KEY_COMMAND_PUSH)) {
            mapping->setCommandPush(settings->at(CFG_KEY_COMMAND_PUSH).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_PUSH
                      << " = '" << mapping->commandPush() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '"
                      << CFG_KEY_COMMAND_PUSH << "' not found" << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_PULL)) {
            mapping->setCommandPull(settings->at(CFG_KEY_COMMAND_PULL).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_PULL
                      << " = '" << mapping->commandPull() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '"
                      << CFG_KEY_COMMAND_PULL << "' not found" << LOG_END

    } catch (toml::type_error& error) {
        LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}


/**
 * Read settings for mapping type encoder
 */
Mapping::ptr Profile::readSettingsForEncoder(int deviceNo, int mapNo, int controlChange, toml::value* settings) {
    LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Read settings for type 'enc'" << LOG_END

    std::shared_ptr<MappingEncoder> mapping = std::make_shared<MappingEncoder>(m_xplane, controlChange);

    // read the four commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_UP)) {
            mapping->setCommandUp(settings->at(CFG_KEY_COMMAND_UP).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_UP << " = '"
                      << mapping->commandUp() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '" << CFG_KEY_COMMAND_UP
                      << "' not found" << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_DOWN)) {
            mapping->setCommandDown(settings->at(CFG_KEY_COMMAND_DOWN).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_DOWN
                      << " = '" << mapping->commandDown() << "'" << LOG_END
        } else
            LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Parameter '"
                      << CFG_KEY_COMMAND_DOWN << "' not found" << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_FAST_UP)) {
            mapping->setCommandFastUp(settings->at(CFG_KEY_COMMAND_FAST_UP).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_FAST_UP
                      << " = '" << mapping->commandFastUp() << "'" << LOG_END
        }

        if (settings->contains(CFG_KEY_COMMAND_FAST_DOWN)) {
            mapping->setCommandFastDown(settings->at(CFG_KEY_COMMAND_FAST_DOWN).as_string().str);
            LOG_DEBUG << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: " << CFG_KEY_COMMAND_FAST_DOWN
                      << " = '" << mapping->commandFastDown() << "'" << LOG_END
        }

    } catch (toml::type_error& error) {
        LOG_ERROR << "Device (" << deviceNo << ") :: Mapping (" << mapNo << ") :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return mapping;
}

} // Namespace XMidiCtrl