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

#include "profile.h"

// Standard
#include <filesystem>
#include <utility>

// XMidiCtrl
#include "logger.h"
#include "map_in_cmd.h"
#include "map_in_drf.h"
#include "map_in_enc.h"
#include "map_in_int.h"
#include "map_in_pnp.h"
#include "map_in_sld.h"
#include "map_out_drf.h"
#include "utils.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile::profile(xplane::ptr xplane)
    : config(std::move(xplane))
{
    m_device_list = std::make_shared<device_list>();
};




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Load the settings for the current aircraft
 */
bool profile::load()
{
    // reset error counter
    m_errors_found = false;

    LOG_DEBUG << "Load aircraft profile" << LOG_END

    // get the filename of the aircraft profile
    std::string filename = get_profile_filename();

    if (filename.empty()) {
        LOG_INFO << "No profile found for current aircraft" << LOG_END
        return false;
    }

    LOG_INFO << "Aircraft Profile '" << filename << "' found" << LOG_END

    // load the profile and create all devices with their mappings
    if (config::load(filename))
        create_device_list();
    else
        return false;

    // open the midi connections
    return m_device_list->open_connections();
}


/**
 * Close the current profile
 */
void profile::close()
{
    m_device_list->clear();
    config::close();
}


/**
 * Returns of the profile contains any errors
 */
bool profile::has_errors() const
{
    return m_errors_found;
}


/**
 * Process all mappings
 */
void profile::process()
{
    // process midi inbound events
    m_device_list->process_inbound_events();

    // process midi outbound mappings
    m_device_list->process_outbound_mappings();
}


//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the profile filename for the current aircraft
 */
std::string profile::get_profile_filename()
{
    std::string filename;

    // 1. check if there is a profile in the aircraft directory
    filename = m_xp->current_aircraft_path() + std::string(FILENAME_PROFILE);

    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // 2. check if there is a profile in the profile directory including the author and ICAO
    filename = m_xp->profiles_path().data() + m_xp->current_aircraft_author() + "_" +
               m_xp->current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // 3. check if there is a profile in the profile directory including the ICAO
    filename =
        m_xp->profiles_path().data() + m_xp->current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // 4. check for the default profile
    filename = m_xp->profiles_path().data() + std::string(FILENAME_PROFILE);
    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // nothing found, return an empty string
    return {};
}


/**
 * Create all defined midi devices
 */
void profile::create_device_list()
{
    // remove all exiting devices first
    m_device_list->clear();

    // only continue if the file contains some settings
    if (m_config.type() == toml::value_t::empty)
        return;

    try {
        // get all devices from the profile
        auto profile_dev = toml::find<std::vector<toml::table>>(m_config, CFG_KEY_DEVICE);
        LOG_INFO << profile_dev.size() << " Device(s) found in aircraft profile" << LOG_END

        // parse every device
        for (int dev_no = 0; dev_no < static_cast<int>(profile_dev.size()); dev_no++) {
            std::string name;
            unsigned int port_in;
            unsigned int port_out;

            // read all parameters for the current device
            toml::value settings_dev = profile_dev[dev_no];

            LOG_DEBUG << "Read settings for device " << dev_no << LOG_END

            try {
                // name
                name = utils::read_string_parameter(settings_dev, CFG_KEY_NAME, false);

                if (name.empty()) {
                    name = "<undefined>";
                    LOG_WARN << "Device " << dev_no << " :: Parameter '" << CFG_KEY_NAME << "' is missing" << LOG_END
                }

                // port in
                    port_in = static_cast<unsigned int>(utils::read_int_parameter(settings_dev, CFG_KEY_PORT_IN));

                    if (port_in < 0) {
                        m_errors_found = true;
                        continue;
                    }

                // port out
                port_out = static_cast<unsigned int>(utils::read_int_parameter(settings_dev, CFG_KEY_PORT_OUT));

                if (port_out < 0) {
                    m_errors_found = true;
                    continue;
                }

                // create device
                std::shared_ptr<device> device = m_device_list->create_device(name, port_in, port_out);

                if (device != nullptr) {
                    // create inbound mappings
                    if (settings_dev.contains(CFG_KEY_MAPPING_IN)) {
                        create_inbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING_IN].as_array(), device);

                    } else if (settings_dev.contains(CFG_KEY_MAPPING)) {
                        LOG_WARN << "Device " << dev_no << " :: Key '" << CFG_KEY_MAPPING << "' "
                                 << "is deprecated, please rename it to '" << CFG_KEY_MAPPING_IN << "'" << LOG_END

                        create_inbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING].as_array(), device);
                    } else
                        LOG_INFO << "Device " << dev_no << " :: No inbound mappings found" << LOG_END

                    // create outbound mappings
                    if (settings_dev.contains(CFG_KEY_MAPPING_OUT))
                        create_outbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING_OUT].as_array(), device);
                    else
                        LOG_INFO << "Device " << dev_no << " :: No outbound mappings found" << LOG_END
                }
            } catch (const std::out_of_range &error) {
                LOG_ERROR << "Device " << dev_no << " :: Error reading profile" << LOG_END
                LOG_ERROR << error.what() << LOG_END

                m_errors_found = true;

            } catch (toml::type_error &error) {
                LOG_ERROR << "Device " << dev_no << " :: Error reading profile" << LOG_END
                LOG_ERROR << error.what() << LOG_END

                m_errors_found = true;
            }
        }
    } catch (std::out_of_range &error) {
        LOG_WARN << "No midi devices found in aircraft profile" << LOG_END
        LOG_WARN << error.what() << LOG_END
    }
}


/**
 * Create the inbound mapping for a device and store it
 */
void profile::create_inbound_mapping(int dev_no, toml::array settings, std::shared_ptr<device> device)
{
    LOG_INFO << "Device " << dev_no << " :: " << settings.size() << " inbound mapping(s) found" << LOG_END

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(settings.size()); map_no++) {
        std::shared_ptr<map_in> mapping;

        LOG_DEBUG << "Device " << dev_no << " :: Read settings for mapping " << map_no << LOG_END

        try {
            map_type type = read_mapping_type(settings[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_type::command:
                    mapping = std::make_shared<map_in_cmd>(m_xp);
                    break;

                case map_type::dataref:
                    mapping = std::make_shared<map_in_drf>(m_xp);
                    break;

                case map_type::slider:
                    mapping = std::make_shared<map_in_sld>(m_xp);
                    break;

                case map_type::push_pull:
                    mapping = std::make_shared<map_in_pnp>(m_xp);
                    break;

                case map_type::encoder:
                    mapping = std::make_shared<map_in_enc>(m_xp);
                    break;

                case map_type::internal:
                    LOG_WARN << "Device " << dev_no << " :: Mapping " << map_no
                             << " :: Mapping type 'int' is currently unsupported" << LOG_END
                    break;

                case map_type::none:
                    LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Invalid mapping type"
                              << LOG_END

                    m_errors_found = true;
                    break;
            }

            if (mapping == nullptr) {
                LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Error reading config" << LOG_END

                m_errors_found = true;
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(settings[map_no]);

            if (mapping->check())
                device->add_inbound_map(mapping);
            else {
                LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Parameters incomplete" << LOG_END

                m_errors_found = true;
            }

        } catch (toml::type_error &error) {
            LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Error reading mapping" << LOG_END
            LOG_ERROR << error.what() << LOG_END

            m_errors_found = true;
        }
    }
}


/**
 * Create the outbound mapping for a device and store it
 */
void profile::create_outbound_mapping(int dev_no, toml::array settings, std::shared_ptr<device> device)
{
    LOG_INFO << "Device " << dev_no << " :: " << settings.size() << " outbound mapping(s) found" << LOG_END

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(settings.size()); map_no++) {
        std::shared_ptr<map_out> mapping;

        LOG_DEBUG << "Device " << dev_no << " :: Mapping " << map_no << " :: Reading config" << LOG_END

        try {
            map_type type = read_mapping_type(settings[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {

                case map_type::dataref:
                    mapping = std::make_shared<map_out_drf>(m_xp);
                    break;

                case map_type::internal:
                    LOG_WARN << "Device " << dev_no << " :: Mapping " << map_no
                             << " :: Mapping type 'int' is currently unsupported" << LOG_END
                    break;

                default:
                    LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Invalid mapping type"
                              << LOG_END

                    m_errors_found = true;
                    break;
            }

            if (mapping == nullptr) {
                LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Error reading config" << LOG_END

                m_errors_found = true;
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(settings[map_no]);

            if (mapping->check())
                device->add_outbound_map(mapping);
            else {
                LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Parameters incomplete" << LOG_END

                m_errors_found = true;
            }

        } catch (toml::type_error &error) {
            LOG_ERROR << "Device " << dev_no << " :: Mapping " << map_no << " :: Error reading mapping" << LOG_END
            LOG_ERROR << error.what() << LOG_END

            m_errors_found = true;
        }
    }
}


/**
 * Translate a type string to an enum value
 */
map_type profile::translateMapTypeStr(std::string_view typeStr)
{
    map_type mapType = map_type::none;

    if (typeStr == CFG_MAPTYPE_COMMAND)
        mapType = map_type::command;
    else if (typeStr == CFG_MAPTYPE_SLIDER)
        mapType = map_type::slider;
    else if (typeStr == CFG_MAPTYPE_DATAREF)
        mapType = map_type::dataref;
    else if (typeStr == CFG_MAPTYPE_PUSH_PULL)
        mapType = map_type::push_pull;
    else if (typeStr == CFG_MAPTYPE_ENCODER)
        mapType = map_type::encoder;
    else if (typeStr == CFG_MAPTYPE_INTERNAL)
        mapType = map_type::internal;

    return mapType;
}


/**
 * Read the mapping type
 */
map_type profile::read_mapping_type(toml::value &settings)
{
    map_type type = map_type::none;

    try {
        // read type
        if (settings.contains(CFG_KEY_TYPE)) {
            std::string_view type_str {settings[CFG_KEY_TYPE].as_string()};
            LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter type = '" << type_str << "'"
                      << LOG_END

            // get the mapping type
            type = translateMapTypeStr(type_str);
        } else {
            LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
            LOG_ERROR << " --> Parameter '" << CFG_KEY_TYPE << "' is missing" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << "Line " << settings.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return type;
}

} // Namespace XMidiCtrl