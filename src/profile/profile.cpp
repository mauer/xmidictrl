//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
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


/**
 * Destructor
 */
profile::~profile()
{
    clear();
}




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
    if (config::load(filename)) {
        m_filename = filename;

        // load general settings for the profile
        m_sl_dataref = toml::find_or<std::string>(m_config, CFG_KEY_SL_DATAREF, "");

        if (!m_sl_dataref.empty())
            LOG_INFO << "SubLayer Mode is active" << LOG_END

        create_device_list();
    } else {
        clear();
        return false;
    }

    // open the midi connections
    return m_device_list->open_connections();
}


/**
 * Close the current profile
 */
void profile::close()
{
    m_device_list->process_outbound_reset();

    clear();
    config::close();
}


/**
 * Return the filename of the loaded profile
 */

std::string_view profile::filename() const
{
    return m_filename;
}


/**
 * Returns of the profile contains any errors
 */
bool profile::has_errors() const
{
    return m_errors_found;
}


/**
 * Returns the title of the current profile
 */
std::string profile::title()
{
    return toml::find_or<std::string>(m_config, CFG_KEY_TITLE, "");
}


/**
 * Returns the version of the current profile
 */
std::string profile::version()
{
    return toml::find_or<std::string>(m_config, CFG_KEY_VERSION, "");
}


/**
 * Return the filename for the aircraft path
 */
std::string profile::get_filename_aircraft_path()
{
    return m_xp->current_aircraft_path() + std::string(FILENAME_PROFILE);
}


/**
 * Return the filename for the profile path
 */
std::string profile::get_filename_profiles_path(bool icao, bool author)
{
    if (icao && author)
        return m_xp->profiles_path().data() + m_xp->current_aircraft_author() + "_" +
               m_xp->current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);
    else if (icao)
        return m_xp->profiles_path().data() + m_xp->current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);
    else
        return m_xp->profiles_path().data() + std::string(FILENAME_PROFILE);
}


/**
 * Process all mappings
 */
void profile::process()
{
    std::string sl_value {};

    // are the sublayers active?
    if (!m_sl_dataref.empty())
        m_xp->datarefs().read(m_sl_dataref, sl_value);

    // process midi inbound events
    m_device_list->process_inbound_events(sl_value);

    // process midi outbound mappings
    m_device_list->process_outbound_mappings(sl_value);
}


//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Clear all data
 */
void profile::clear()
{
    m_sl_dataref.clear();

    m_filename.clear();
    m_device_list->clear();
}


/**
 * Get the profile filename for the current aircraft
 */
std::string profile::get_profile_filename()
{
    std::string filename;

    // 1. check if there is a profile in the aircraft directory
    filename = get_filename_aircraft_path();

    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // 2. check if there is a profile in the profile directory including the author and ICAO
    filename = get_filename_profiles_path(true, true);

    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // 3. check if there is a profile in the profile directory including the ICAO
    filename = get_filename_profiles_path(true, false);

    LOG_DEBUG << " --> Search for aircraft profile '" << filename << "'" << LOG_END
    if (std::filesystem::exists(filename)) {
        return filename;
    }

    // 4. check for the default profile
    filename = get_filename_profiles_path(false, false);
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
            int port_in;
            int port_out;
            mode_out mode_out;

            // read all parameters for the current device
            toml::value settings_dev = profile_dev[dev_no];

            LOG_DEBUG << "Read settings for device " << dev_no << LOG_END

            try {
                // name
                name = utils::toml_read_string(settings_dev, CFG_KEY_NAME, false);

                if (name.empty()) {
                    name = "<undefined>";
                    LOG_WARN << "Device " << dev_no << " :: Parameter '" << CFG_KEY_NAME << "' is missing" << LOG_END
                }

                // port in
                port_in = utils::toml_read_int(settings_dev, CFG_KEY_PORT_IN);

                if (port_in < 0) {
                    m_errors_found = true;
                    continue;
                }

                // port out
                port_out = utils::toml_read_int(settings_dev, CFG_KEY_PORT_OUT);

                if (port_out < 0) {
                    m_errors_found = true;
                    continue;
                }

                // mode outbound
                mode_out = utils::mode_out_from_int(utils::toml_read_int(settings_dev,
                                                                         CFG_KEY_MODE_OUT,
                                                                         false));

                // create device
                std::shared_ptr<device> device = m_device_list->create_device(name, static_cast<int>(port_in),
                                                                              static_cast<int>(port_out), mode_out);

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
void profile::create_inbound_mapping(int dev_no, toml::array settings, const std::shared_ptr<device> &device)
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
                LOG_ERROR << "Line " << settings[map_no].location().line() << " :: "
                          << settings[map_no].location().line_str() << LOG_END
                LOG_ERROR << " --> Error reading mapping" << LOG_END
                m_errors_found = true;
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(settings[map_no]);

            if (mapping->check()) {
                device->add_inbound_map(mapping);
            } else {
                LOG_ERROR << "Line " << settings[map_no].location().line() << " :: "
                          << settings[map_no].location().line_str() << LOG_END
                LOG_ERROR << " --> Parameters incomplete" << LOG_END

                m_errors_found = true;
            }

        } catch (toml::type_error &error) {
            LOG_ERROR << "Line " << settings[map_no].location().line() << " :: "
                      << settings[map_no].location().line_str() << LOG_END
            LOG_ERROR << " --> Error reading config" << LOG_END
            LOG_ERROR << error.what() << LOG_END

            m_errors_found = true;
        }
    }
}


/**
 * Create the outbound mapping for a device and store it
 */
void profile::create_outbound_mapping(int dev_no, toml::array settings, const std::shared_ptr<device> &device)
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