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
#include "map_in_cmd.h"
#include "map_in_drf.h"
#include "map_in_enc.h"
#include "map_in_pnp.h"
#include "map_in_sld.h"
#include "map_out_drf.h"
#include "utils.h"
#include "toml_utils.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile::profile(xplane *xp,
                 std::shared_ptr<message_handler> messages,
                 std::shared_ptr<settings> settings)
    : config(xp),
      m_settings(std::move(settings))
{
    m_profile_msg = std::make_unique<message_handler>(std::move(messages));
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
    // reset all messages
    m_profile_msg->clear();
    m_profile_msg->debug("Load aircraft profile");

    // get the filename of the aircraft profile
    std::string filename = find_profile();

    if (filename.empty()) {
        m_profile_msg->info("No profile found for current aircraft");
        return true;
    }

    m_profile_msg->info("Aircraft Profile '%s' found", filename.c_str());

    // load the profile and create all devices with their mappings
    if (config::load(filename)) {
        m_filename = filename;

        // load general settings for the profile
        m_sl_dataref = toml::find_or<std::string>(m_config, CFG_KEY_SL_DATAREF, "");

        if (!m_sl_dataref.empty())
            m_profile_msg->info("Sublayer mode activated");

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
    return m_profile_msg->has_errors();
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


std::string_view profile::sl_dataref() const
{
    return m_sl_dataref;
}


/**
 * Process all mappings
 */
void profile::process()
{
    // process midi outbound mappings
    m_device_list->process_outbound_mappings();
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

    m_profile_msg->clear();
    m_device_list->clear();
}


/**
 * Get the profile filename for the current aircraft
 */
std::string profile::find_profile()
{
    std::string filename;

    // 1. check if there is a profile in the aircraft directory
    filename = get_filename_aircraft_path();

    m_profile_msg->debug(" --> Search for aircraft profile '%s'", filename.c_str());
    if (std::filesystem::exists(filename))
        return filename;

    // 2. check if there is a profile in the profile directory including the author and ICAO
    filename = get_filename_profiles_path(true, true);

    m_profile_msg->debug(" --> Search for aircraft profile '%s'", filename.c_str());
    if (std::filesystem::exists(filename))
        return filename;

    // 3. check if there is a profile in the profile directory including the ICAO
    filename = get_filename_profiles_path(true, false);

    m_profile_msg->debug(" --> Search for aircraft profile '%s'", filename.c_str());
    if (std::filesystem::exists(filename))
        return filename;

    // 4. check for the common profile (if activated)
    if (m_settings->common_profile()) {
        filename = get_filename_profiles_path(false, false);

        m_profile_msg->debug(" --> Search for aircraft profile '%s'", filename.c_str());
        if (std::filesystem::exists(filename))
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
    toml_utils utils(m_profile_msg.get());

    // remove all exiting devices first
    m_device_list->clear();

    // only continue if the file contains some settings
    if (m_config.type() == toml::value_t::empty)
        return;

    try {
        // get all devices from the profile
        auto profile_dev = toml::find<std::vector<toml::table>>(m_config, CFG_KEY_DEVICE);

        m_profile_msg->info("%i Device(s) found in aircraft profile", profile_dev.size());

        // parse every device
        for (int dev_no = 0; dev_no < static_cast<int>(profile_dev.size()); dev_no++) {
            std::string name;
            int port_in;
            int port_out;
            mode_out mode_out;

            // read all parameters for the current device
            toml::value settings_dev = profile_dev[dev_no];

            m_profile_msg->debug("Read settings for device %i", dev_no);

            try {
                // name
                name = utils.read_string(settings_dev, CFG_KEY_NAME, false);

                if (name.empty()) {
                    name = "<undefined>";
                    m_profile_msg->warn("Device %i :: Parameter '%s' is missing", dev_no, CFG_KEY_NAME);
                }

                // port in
                port_in = utils.read_int(settings_dev, CFG_KEY_PORT_IN);

                if (port_in < 0) {
                    //m_errors_found = true;
                    continue;
                }

                // port out
                port_out = utils.read_int(settings_dev, CFG_KEY_PORT_OUT);

                if (port_out < 0) {
                    //m_errors_found = true;
                    continue;
                }

                // mode outbound
                mode_out = utils::mode_out_from_int(utils.read_int(settings_dev,
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
                        m_profile_msg->warn("Device %i :: Key '%s' is deprecated, please rename it to '%s'",
                                            dev_no,
                                            CFG_KEY_MAPPING,
                                            CFG_KEY_MAPPING_IN);
                        create_inbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING].as_array(), device);
                    } else
                        m_profile_msg->info("Device %i :: No inbound mappings found", dev_no);

                    // create outbound mappings
                    if (settings_dev.contains(CFG_KEY_MAPPING_OUT))
                        create_outbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING_OUT].as_array(), device);
                    else
                        m_profile_msg->info("Device %i :: No outbound mappings found", dev_no);
                }
            } catch (const std::out_of_range &error) {
                m_profile_msg->error("Device %i :: Error reading profile", dev_no);
                m_profile_msg->error(error.what());
            } catch (toml::type_error &error) {
                m_profile_msg->error("Device %i :: Error reading profile", dev_no);
                m_profile_msg->error(error.what());
            }
        }
    } catch (std::out_of_range &error) {
        m_profile_msg->warn("No MIDI devices found in aircraft profile");
        m_profile_msg->warn(error.what());
    }
}


/**
 * Create the inbound mapping for a device and store it
 */
void profile::create_inbound_mapping(int dev_no, toml::array settings, const std::shared_ptr<device> &device)
{
    m_profile_msg->info("Device %i :: %i inbound mapping(s) found", dev_no, settings.size());

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(settings.size()); map_no++) {
        std::shared_ptr<map_in> mapping;

        m_profile_msg->debug("Device %i :: Read settings for mapping %i", dev_no, map_no);

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

                case map_type::encoder:
                    mapping = std::make_shared<map_in_enc>(m_xp);
                    break;

                case map_type::internal:
                    m_profile_msg->warn("Device %i :: Mapping %i :: Mapping type 'int' is currently unsupported", dev_no, map_no);
                    break;

                case map_type::push_pull:
                    mapping = std::make_shared<map_in_pnp>(m_xp);
                    break;

                case map_type::slider:
                    mapping = std::make_shared<map_in_sld>(m_xp);
                    break;

                case map_type::none:
                    m_profile_msg->error("Device %i :: Mapping %i :: Invalid mapping type", dev_no, map_no);
                    break;
            }

            if (mapping == nullptr) {
                m_profile_msg->error("Line %i :: %s", settings[map_no].location().line(), settings[map_no].location().line_str().c_str());
                m_profile_msg->error(" --> Error reading mapping");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(m_profile_msg.get(), settings[map_no]);

            if (mapping->check()) {
                device->add_inbound_map(mapping);
            } else {
                m_profile_msg->error("Line %i :: %s", settings[map_no].location().line(), settings[map_no].location().line_str().c_str());
                m_profile_msg->error(" --> Parameters incomplete or incorrect");
            }
        } catch (toml::type_error &error) {
            m_profile_msg->error("Line %i :: %s", settings[map_no].location().line(), settings[map_no].location().line_str().c_str());
            m_profile_msg->error(" --> Error reading config");
            m_profile_msg->error(error.what());
        }
    }
}


/**
 * Create the outbound mapping for a device and store it
 */
void profile::create_outbound_mapping(int dev_no, toml::array settings, const std::shared_ptr<device> &device)
{
    m_profile_msg->info("Device %i :: %i outbound mapping(s) found", dev_no, settings.size());

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(settings.size()); map_no++) {
        std::shared_ptr<map_out> mapping;

        m_profile_msg->debug("Device %i :: Mapping %i :: Reading config", dev_no, map_no);

        try {
            map_type type = read_mapping_type(settings[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {

                case map_type::dataref:
                    mapping = std::make_shared<map_out_drf>(m_xp);
                    break;

                case map_type::internal:
                    m_profile_msg->warn("Device %i :: Mapping %i :: Mapping type 'int' is currently unsupported", dev_no, map_no);
                    break;

                default:
                    m_profile_msg->error("Device %i :: Mapping %i :: Invalid mapping type", dev_no, map_no);
                    break;
            }

            if (mapping == nullptr) {
                m_profile_msg->error("Device %i :: Mapping %i :: Error reading config", dev_no, map_no);
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(m_profile_msg.get(), settings[map_no]);

            if (mapping->check())
                device->add_outbound_map(mapping);
            else
                m_profile_msg->error("Device %i :: Mapping %i :: Parameters incomplete or incorrect", dev_no, map_no);

        } catch (toml::type_error &error) {
            m_profile_msg->error("Device %i :: Mapping %i :: Error reading mapping", dev_no, map_no);
            m_profile_msg->error(error.what());
        }
    }
}


/**
 * Translate a type string to an enum value
 */
map_type profile::translate_map_type(std::string_view type_str)
{
    map_type type = map_type::none;

    if (type_str == CFG_MAPTYPE_COMMAND)
        type = map_type::command;
    else if (type_str == CFG_MAPTYPE_SLIDER)
        type = map_type::slider;
    else if (type_str == CFG_MAPTYPE_DATAREF)
        type = map_type::dataref;
    else if (type_str == CFG_MAPTYPE_PUSH_PULL)
        type = map_type::push_pull;
    else if (type_str == CFG_MAPTYPE_ENCODER)
        type = map_type::encoder;
    else if (type_str == CFG_MAPTYPE_INTERNAL)
        type = map_type::internal;

    return type;
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

            m_profile_msg->debug(" --> Line %i :: Parameter type = '%s'", settings.location().line(), type_str.data());

            // get the mapping type
            type = translate_map_type(type_str);
        } else {
            m_profile_msg->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
            m_profile_msg->error(" --> Parameter '%s' is missing", CFG_KEY_TYPE);
        }
    } catch (toml::type_error &error) {
        m_profile_msg->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_profile_msg->error("Line %i :: Error reading mapping", settings.location().line());
        m_profile_msg->error(error.what());
    }

    return type;
}

} // Namespace xmidictrl