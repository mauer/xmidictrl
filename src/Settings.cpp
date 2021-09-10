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
#include "XPLMUtilities.h"

// X-Plane SDK Utils
#include "PluginLogger.h"

// XMidiCtrl
#include "Settings.h"
#include "Global.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Settings::Settings() = default;


/**
 * Destructor
 */
Settings::~Settings() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Reload the settings for the current aircraft
 */
void Settings::reloadSettingsForAircraft() {
    LOG_DEBUG << "SETTINGS :: Reload settings for aircraft" << LOG_END

    // reset aircraft settings
    m_aircraftConfig = toml::value();

    // load config for current aircraft
    std::string fileName = getAircraftConfigFileName();

    if (!fileName.empty()) {
        if (!std::filesystem::exists(fileName))
        {
            LOG_INFO << "SETTINGS :: Config file for aircraft not found" << LOG_END
            return;
        }

        try {
            // load settings
            m_aircraftConfig = toml::parse(fileName);

            LOG_INFO << "SETTINGS :: Config for aircraft loaded successfully" << LOG_END

        } catch (const toml::syntax_error& error) {
            LOG_ERROR << "SETTINGS :: Error parsing config file for aircraft" << LOG_END
            LOG_ERROR << error.what() << LOG_END

        } catch (const std::runtime_error& error) {
            LOG_ERROR << "SETTINGS :: Error opening config file for aircraft (" << fileName << ")" << LOG_END
            LOG_ERROR << error.what() << LOG_END
        }
    }
}


/**
 * Create a list with all configured midi devices
 */
std::vector<DeviceSettings> Settings::deviceList() {
    std::vector<DeviceSettings> deviceList;
    DeviceSettings deviceSettings;
    
    // only continue if the file contains some settings
    if (m_aircraftConfig.type() == toml::value_t::empty) {
        return deviceList;
    }

    try {
        // get all devices
        auto devices = toml::find<std::vector<toml::table>>(m_aircraftConfig, CFG_KEY_DEVICE);

        LOG_DEBUG << "SETTINGS :: " << devices.size() << " Devices found in config" << LOG_END
        
        // parse every device
        for (int i = 0; i < static_cast<int>(devices.size()); i++) {
            deviceSettings = {};

            try {
                // general settings
                deviceSettings.name    = devices[i][CFG_KEY_NAME].as_string();
                deviceSettings.portIn  = static_cast<int>( devices[i][CFG_KEY_PORT_IN].as_integer() );
                deviceSettings.portOut = static_cast<int>( devices[i][CFG_KEY_PORT_OUT].as_integer() );

                LOG_DEBUG << "SETTINGS :: Device (" << i << ") :: Name     = " << deviceSettings.name << LOG_END
                LOG_DEBUG << "SETTINGS :: Device (" << i << ") :: Port In  = " << deviceSettings.portIn << LOG_END
                LOG_DEBUG << "SETTINGS :: Device (" << i << ") :: Port Out = " << deviceSettings.portOut << LOG_END

                // parse note mappings
                deviceSettings.mapping = mappingForDevice(devices[i]["mapping"].as_array());

            } catch (const std::out_of_range& error) {
                LOG_ERROR << "SETTINGS :: Error reading config for midi device (" << i << ")" << LOG_END
                LOG_ERROR << error.what() << LOG_END
            }

            deviceList.push_back(deviceSettings);
        }
    } catch (std::out_of_range& error) {
        LOG_WARN << "SETTINGS :: No midi devices found in config" << LOG_END
        LOG_WARN << error.what() << LOG_END
    }

    return deviceList;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the config filename for the currently loaded aircraft
 */
std::string Settings::getAircraftConfigFileName() {
    std::string fileName;
    
    char aircraftFileName[256];
    char aircraftPath[512];

    // aircraft with index 0 is the user aircraft
    XPLMGetNthAircraftModel(0, aircraftFileName, aircraftPath);

    LOG_DEBUG << "SETTINGS :: Current Aircraft :: Filename = " << aircraftFileName << LOG_END
    LOG_DEBUG << "SETTINGS :: Current Aircraft :: Path = " << aircraftPath << LOG_END

    if (!std::string(aircraftFileName).empty()) {
        // remove filename from path
        fileName = std::string(aircraftPath);
        fileName = fileName.substr(0, fileName.size() - std::string(aircraftFileName).size());
        fileName = fileName + XMIDICTRL_CONFIG_FILENAME;
    }

    LOG_DEBUG << "SETTINGS :: Name for aircraft config file = " << fileName << LOG_END
 
    return fileName;
}


/**
 * Create a list with all mappings for the current midi device
 */
std::map<int, MidiMapping> Settings::mappingForDevice(toml::array settings) {
    std::map<int, MidiMapping> mappingList;
    MidiMapping mapping;
    std::string typeStr;

    LOG_DEBUG << "SETTINGS :: Mappings found = " << settings.size() << LOG_END;

    // parse each mapping entry
    for (int i = 0; i < static_cast<int>(settings.size()); i++) {
        mapping = {};

        try {
            if (settings[i].contains(CFG_KEY_CC))
                mapping.controlChange = static_cast<int>( settings[i][CFG_KEY_CC].as_integer() );

            if (settings[i].contains(CFG_KEY_TYPE))
                typeStr = settings[i][CFG_KEY_TYPE].as_string();

            LOG_DEBUG << "SETTINGS :: Mapping (" << i << ") :: CC   = " << mapping.controlChange << LOG_END
            LOG_DEBUG << "SETTINGS :: Mapping (" << i << ") :: Type = " << typeStr << LOG_END
            
            // get the mapping type
            mapping.type = translateMapTypeStr(typeStr);

            // depending on the mapping type, we have to read some additional settings
            switch (mapping.type) {
                case MappingType::Command:
                    if (!readSettingsForCommand(&mapping, &settings[i]))
                        continue;
                    
                    break;

                    case MappingType::DataRef:
                        if (!readSettingsForDataRef(&mapping, &settings[i]))
                            continue;

                        break;

                        case MappingType::Slider:
                            if (!readSettingsForSlider(&mapping, &settings[i]))
                                continue;

                            break;

                            case MappingType::PushAndPull:
                                if (!readSettingsForPushAndPull(&mapping, &settings[i]))
                                    continue;
                    
                                break;

                                case MappingType::Encoder:
                                    if (!readSettingsForEncoder(&mapping, &settings[i]))
                                        continue;

                                    break;

                                    case MappingType::Internal:
                                        break;

                                    case MappingType::None:
                                        LOG_WARN << "SETTINGS :: Invalid mapping type" << LOG_END
                                        continue;
            }

            mappingList.emplace(mapping.controlChange, mapping);

        } catch (toml::type_error& error) {
            LOG_ERROR << "SETTINGS :: Error reading mapping" << LOG_END
            LOG_ERROR << error.what() << LOG_END
        }
    }
                
    return mappingList;
}


/**
 * Translate a type string to an enum value
 */
MappingType Settings::translateMapTypeStr(std::string_view typeStr) {
    MappingType mapType = MappingType::None;

    if (typeStr == XMIDICTRL_MAPTYPE_COMMAND)
        mapType = MappingType::Command;
    else if (typeStr == XMIDICTRL_MAPTYPE_SLIDER)
        mapType = MappingType::Slider;
    else if (typeStr == XMIDICTRL_MAPTYPE_DATAREF)
        mapType = MappingType::DataRef;
    else if (typeStr == XMIDICTRL_MAPTYPE_PUSHANDPULL)
        mapType = MappingType::PushAndPull;
    else if (typeStr == XMIDICTRL_MAPTYPE_ENCODER)
        mapType = MappingType::Encoder;
    else if (typeStr == XMIDICTRL_MAPTYPE_INTERNAL)
        mapType = MappingType::Internal;

    return mapType;
}


/**
 * Read settings for mapping type command
 */
bool Settings::readSettingsForCommand(MidiMapping* mapping, toml::value* settings) {
    bool result = false;

    LOG_DEBUG << "SETTINGS :: Read settings for command" << LOG_END

    // read the actual command
    try {
        if (settings->contains(CFG_KEY_COMMAND))
            mapping->command = settings->at(CFG_KEY_COMMAND).as_string();

        LOG_DEBUG << "SETTINGS :: Command = " << mapping->command << LOG_END

        if (!mapping->command.empty())
            result = true;

    } catch (toml::type_error& error) {
        LOG_ERROR << "SETTINGS :: Error reading mapping for CC" << mapping->controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return result;
}


/**
 * Read settings for mapping type dataref
 */
bool Settings::readSettingsForDataRef(MidiMapping* mapping, toml::value* settings) {
    bool result = false;

    LOG_DEBUG << "SETTINGS :: Read settings for dataref" << LOG_END

    try {
        // read the actual dataref
        if (settings->contains(CFG_KEY_DATAREF))
            mapping->dataRef = settings->at(CFG_KEY_DATAREF).as_string();

        LOG_DEBUG << "SETTINGS :: Dataref   = " << mapping->dataRef << LOG_END

        // read on value
        if (settings->contains(CFG_KEY_VALUE_ON))
            mapping->valueOn = settings->at(CFG_KEY_VALUE_ON).as_string();

        LOG_DEBUG << "SETTINGS :: Value On  = " << mapping->valueOn << LOG_END


        // read off value
        if (settings->contains(CFG_KEY_VALUE_OFF))
            mapping->valueOff = settings->at(CFG_KEY_VALUE_OFF).as_string();

        LOG_DEBUG << "SETTINGS :: Value Off = " << mapping->valueOff << LOG_END

        if ((!mapping->dataRef.empty()) && (!mapping->valueOn.empty()) && (!mapping->valueOff.empty()))
            result = true;

    } catch (toml::type_error& error) {
        LOG_ERROR << "SETTINGS :: Error reading mapping for CC" << mapping->controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return result;
}


/**
 * Read settings for mapping type slider
 */
bool Settings::readSettingsForSlider(MidiMapping* mapping, toml::value* settings) {
    bool result = false;

    LOG_DEBUG << "SETTINGS :: Read settings for slider" << LOG_END

    // read the four commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_UP))
            mapping->commandUp = settings->at(CFG_KEY_COMMAND_UP).as_string();

        LOG_DEBUG << "SETTINGS :: Command Up   = " << mapping->commandUp << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_DOWN))
            mapping->commandDown = settings->at(CFG_KEY_COMMAND_DOWN).as_string();

        LOG_DEBUG << "SETTINGS :: Command Down = " << mapping->commandDown << LOG_END

        if ((!mapping->commandUp.empty()) && (!mapping->commandDown.empty()))
            result = true;

    } catch (toml::type_error& error) {
        LOG_ERROR << "SETTINGS :: Error reading mapping for CC" << mapping->controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return result;
}


/**
 * Read settings for mapping type push and pull
 */
bool Settings::readSettingsForPushAndPull(MidiMapping* mapping, toml::value* settings) {
    bool result = false;

    LOG_DEBUG << "SETTINGS :: Read settings for push and pull" << LOG_END

    // read the two commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_PUSH))
            mapping->commandPush = settings->at(CFG_KEY_COMMAND_PUSH).as_string();

        LOG_DEBUG << "SETTINGS :: Command Push = " << mapping->commandPush << LOG_END
        
        if (settings->contains(CFG_KEY_COMMAND_PULL))
            mapping->commandPull = settings->at(CFG_KEY_COMMAND_PULL).as_string();

        LOG_DEBUG << "SETTINGS :: Command Pull = " << mapping->commandPull << LOG_END

        if ((!mapping->commandPush.empty()) && (!mapping->commandPull.empty()))
            result = true;

    } catch (toml::type_error& error) {
        LOG_ERROR << "SETTINGS :: Error reading mapping for CC" << mapping->controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return result;
}


/**
 * Read settings for mapping type encoder
 */
bool Settings::readSettingsForEncoder(MidiMapping* mapping, toml::value* settings) {
    bool result = false;

    LOG_DEBUG << "SETTINGS :: Read settings for encoder" << LOG_END

    // read the four commands
    try {
        if (settings->contains(CFG_KEY_COMMAND_UP))
            mapping->commandUp = settings->at(CFG_KEY_COMMAND_UP).as_string();

        LOG_DEBUG << "SETTINGS :: Command Up   = " << mapping->commandUp << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_DOWN))
            mapping->commandDown = settings->at(CFG_KEY_COMMAND_DOWN).as_string();

        LOG_DEBUG << "SETTINGS :: Command Down = " << mapping->commandDown << LOG_END

        if (settings->contains(CFG_KEY_COMMAND_UP_FAST)) {
            mapping->commandUpFast = settings->at(CFG_KEY_COMMAND_UP_FAST).as_string();

            LOG_DEBUG << "SETTINGS :: Command Up (fast)   = " << mapping->commandUpFast << LOG_END
        } else {
            mapping->commandUpFast = mapping->commandUp;
        }
        
        if (settings->contains(CFG_KEY_COMMAND_DOWN_FAST)) {
            mapping->commandDownFast = settings->at(CFG_KEY_COMMAND_DOWN_FAST).as_string();

            LOG_DEBUG << "SETTINGS :: Command Down (fast) = " << mapping->commandDownFast << LOG_END
        } else {
            mapping->commandDownFast = mapping->commandDown;
        }

        if ((!mapping->commandUp.empty()) && (!mapping->commandUpFast.empty()) &&
        (!mapping->commandDown.empty()) && (!mapping->commandDownFast.empty()))
            result = true;

    } catch (toml::type_error& error) {
        LOG_ERROR << "SETTINGS :: Error reading mapping for CC" << mapping->controlChange << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return result;
}

} // Namespace XMidiCtrl