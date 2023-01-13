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

// XMidiCtrl
#include "map_in_cmd.h"
#include "map_in_drf.h"
#include "map_in_enc.h"
#include "map_in_pnp.h"
#include "map_in_sld.h"
#include "map_out_drf.h"
#include "midi_logger.h"
#include "conversions.h"
#include "toml_utils.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile::profile(text_logger &in_text_log, midi_logger &in_midi_log, xplane &in_xp, settings &in_settings)
    : config(in_xp),
      m_settings(in_settings),
      m_plugin_log(in_text_log),
      m_midi_log(in_midi_log)
{
    // create my own logger for errors and warnings
    m_profile_log = std::make_unique<text_logger>(&in_text_log);
    m_profile_log->set_log_info(false);

    m_device_list = std::make_unique<device_list>();
}


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
    clear();

    m_profile_log->debug("Load aircraft profile");

    // get the filename of the aircraft profile
    std::string filename = find_profile();

    if (filename.empty()) {
        m_profile_log->info("No profile found for current aircraft");
        return true;
    }

    m_profile_log->info("Aircraft Profile '" + filename + "' found");

    // load the profile and create all devices with their mappings
    if (load_file(*m_profile_log, filename)) {
        m_filename = filename;

        // load general settings for the profile
        m_sl_dataref = toml::find_or<std::string>(m_config, CFG_KEY_SL_DATAREF, "");

        if (!m_sl_dataref.empty()) {
            m_profile_log->info("Sublayer mode activated");

            if (!xp().datarefs().check(m_sl_dataref)) {
                m_profile_log->error("Dataref '" + std::string(m_sl_dataref) + "' not found");
                return false;
            }
        }

        create_device_list();
    } else {
        clear();
        return false;
    }

    // open the midi connections
    if (m_device_list->open_connections()) {
        m_loaded = true;
        return true;
    } else {
        return false;
    }
}


/**
 * Close the current profile
 */
void profile::close()
{
    m_device_list->process_outbound_reset();

    clear();
    close_file(*m_profile_log);

    m_loaded = false;
}


/**
 * Return if a profile is loaded
 */
bool profile::loaded() const
{
    return m_loaded;
}


/**
 * Return the filename of the loaded profile
 */

std::string_view profile::filename() const
{
    return m_filename;
}


/**
 * Return the log
 */
text_logger &profile::log()
{
    return *m_profile_log;
}


/**
 * Returns of the profile contains any errors
 */
bool profile::has_errors() const
{
    return m_profile_log->has_errors();
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
std::string profile::get_filename_aircraft_path(filename_prefix in_prefix)
{
    switch (in_prefix) {
        case filename_prefix::icao:
            return xplane::current_aircraft_path() + xp().current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

        case filename_prefix::acf_name:
            return xplane::current_aircraft_path() + xp().current_aircraft_acf_name() + "_"
                   + std::string(FILENAME_PROFILE);

        default:
            return xplane::current_aircraft_path() + std::string(FILENAME_PROFILE);
    }
}


/**
 * Return the filename for the profile path
 */
std::string profile::get_filename_profiles_path(filename_prefix in_prefix)
{
    switch (in_prefix) {
        case filename_prefix::icao:
            return xp().profiles_path() + xp().current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

        case filename_prefix::acf_name:
            return xp().profiles_path() + xp().current_aircraft_acf_name() + "_" + std::string(FILENAME_PROFILE);

        default:
            return xp().profiles_path() + std::string(FILENAME_PROFILE);
    }
}


/**
 * Return the name of the sublayer dataref
 */
std::string_view profile::sl_dataref() const
{
    return m_sl_dataref;
}


/**
 * Process all outbound mappings
 */
void profile::process(text_logger &in_log)
{
    if (!m_init_send) {
        // process init mappings
        m_device_list->process_init_mappings(in_log);
        m_init_send = true;
    }

    // process midi outbound mappings
    m_device_list->process_outbound_mappings(in_log);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Clear all data
 */
void profile::clear()
{
    m_loaded = false;
    m_init_send = false;

    m_config = toml::value();

    m_sl_dataref.clear();
    m_filename.clear();

    m_profile_log->clear();
    m_device_list->clear();
}


/**
 * Get the profile filename for the current aircraft
 */
std::string profile::find_profile()
{
    std::string filename;

    // check if there is a profile in the aircraft directory
    filename = get_filename_aircraft_path(filename_prefix::none);

    m_profile_log->debug(" --> Search for aircraft profile '" + filename + "'");
    if (std::filesystem::exists(filename))
        return filename;

    // check if there is a profile in the aircraft directory including the ICAO
    filename = get_filename_aircraft_path(filename_prefix::icao);

    m_profile_log->debug(" --> Search for aircraft profile '" + filename + "'");
    if (std::filesystem::exists(filename))
        return filename;

    // check if there is a profile in the aircraft directory including the acf name
    filename = get_filename_aircraft_path(filename_prefix::acf_name);

    m_profile_log->debug(" --> Search for aircraft profile '" + filename + "'");
    if (std::filesystem::exists(filename))
        return filename;

    // check if there is a profile in the profile directory including the ICAO
    filename = get_filename_profiles_path(filename_prefix::icao);

    m_profile_log->debug(" --> Search for aircraft profile '" + filename + "'");
    if (std::filesystem::exists(filename))
        return filename;

    // check if there is a profile in the profile directory including the acf name
    filename = get_filename_profiles_path(filename_prefix::acf_name);

    m_profile_log->debug(" --> Search for aircraft profile '" + filename + "'");
    if (std::filesystem::exists(filename))
        return filename;

    // check for the common profile (if activated)
    if (m_settings.use_common_profile()) {
        filename = get_filename_profiles_path(filename_prefix::none);

        m_profile_log->debug(" --> Search for aircraft profile '" + filename + "'");
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
    // remove all exiting devices first
    m_device_list->clear();

    // only continue if the file contains some settings
    if (m_config.type() == toml::value_t::empty)
        return;

    try {
        // get all devices from the profile
        auto profile_dev = toml::find<std::vector<toml::table>>(m_config, CFG_KEY_DEVICE);

        m_profile_log->info(std::to_string(profile_dev.size()) + " Device(s) found in aircraft profile");

        // parse every device
        for (size_t dev_no = 0; dev_no < profile_dev.size(); dev_no++) {
            std::string name;
            int port_in;
            int port_out;
            mode_out mode_out;
            encoder_mode default_enc_mode;

            // read all parameters for the current device
            toml::value settings_dev = profile_dev[dev_no];

            m_profile_log->debug("Read settings for device " + std::to_string(dev_no));

            try {
                // name
                name = toml_utils::read_string(*m_profile_log, settings_dev, CFG_KEY_NAME, false);

                if (name.empty()) {
                    name = "<undefined>";
                    m_profile_log->warn("Device " + std::to_string(dev_no) + " :: "
                                        + "Parameter '" + CFG_KEY_NAME + "' is missing");
                }

                // port in
                port_in = toml_utils::read_int(*m_profile_log, settings_dev, CFG_KEY_PORT_IN);

                if (port_in < 0) {
                    m_profile_log->error("Invalid inbound port '" + std::to_string(port_in) + "'");
                    continue;
                }

                // port out
                port_out = toml_utils::read_int(*m_profile_log, settings_dev, CFG_KEY_PORT_OUT);

                if (port_out < 0) {
                    m_profile_log->error("Invalid outbound port '" + std::to_string(port_out) + "'");
                    continue;
                }

                // mode outbound
                mode_out = conversions::mode_out_from_int(toml_utils::read_int(*m_profile_log,
                                                                               settings_dev,
                                                                               CFG_KEY_MODE_OUT,
                                                                               false));

                // default encoder mode
                default_enc_mode = conversions::encoder_mode_from_code(toml_utils::read_string(*m_profile_log,
                                                                                               settings_dev,
                                                                                               CFG_KEY_ENCODER_MODE,
                                                                                               false));

                // create device
                std::shared_ptr<device> device = m_device_list->create_device(m_plugin_log,
                                                                              m_midi_log,
                                                                              name,
                                                                              port_in,
                                                                              port_out,
                                                                              mode_out,
                                                                              default_enc_mode);

                if (device != nullptr) {
                    // create init mappings
                    if (settings_dev.contains(CFG_KEY_MAPPING_INIT))
                        create_init_mapping(dev_no, settings_dev[CFG_KEY_MAPPING_INIT].as_array(), device);

                    // create inbound mappings
                    if (settings_dev.contains(CFG_KEY_MAPPING_IN)) {
                        create_inbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING_IN].as_array(), device);
                    } else if (settings_dev.contains(CFG_KEY_MAPPING)) {
                        m_profile_log->warn("Device " + std::to_string(dev_no) + " :: "
                                            + "Key '" + CFG_KEY_MAPPING + "' is deprecated, please rename it to '"
                                            + CFG_KEY_MAPPING_IN + "'");
                        create_inbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING].as_array(), device);
                    } else
                        m_profile_log->info("Device " + std::to_string(dev_no) + " :: No inbound mappings found");

                    // create outbound mappings
                    if (settings_dev.contains(CFG_KEY_MAPPING_OUT))
                        create_outbound_mapping(dev_no, settings_dev[CFG_KEY_MAPPING_OUT].as_array(), device);
                    else
                        m_profile_log->info("Device " + std::to_string(dev_no) + " :: No outbound mappings found");
                }
            } catch (const std::out_of_range &error) {
                m_profile_log->error("Device " + std::to_string(dev_no)+ " :: Error reading profile");
                m_profile_log->error(error.what());
            } catch (toml::type_error &error) {
                m_profile_log->error("Device " + std::to_string(dev_no) + " :: Error reading profile");
                m_profile_log->error(error.what());
            }
        }
    } catch (std::out_of_range &error) {
        m_profile_log->warn("No MIDI devices found in aircraft profile");
        m_profile_log->warn(error.what());
    }
}


/**
 * Create the init mapping for a device and store it
 */
void profile::create_init_mapping(int in_dev_no, toml::array in_settings, const std::shared_ptr<device> &out_device)
{
    m_profile_log->info("Device " + std::to_string(in_dev_no) + " :: "
                        + std::to_string(in_settings.size()) + " init mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_settings.size()); map_no++) {
        std::shared_ptr<map_init> mapping;

        m_profile_log->debug("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Reading config");

        try {
            mapping = std::make_shared<map_init>(xp());

            // read the settings and check if everything we need was defined
            mapping->read_config(*m_profile_log, in_settings[map_no]);

            if (mapping->check(*m_profile_log))
                out_device->add_init_map(mapping);
            else
                m_profile_log->error("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Parameters incomplete or incorrect");

        } catch (toml::type_error &error) {
            m_profile_log->error("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Error reading mapping");
            m_profile_log->error(error.what());
        }
    }
}


/**
 * Create the inbound mapping for a device and store it
 */
void profile::create_inbound_mapping(int in_dev_no, toml::array in_settings, const std::shared_ptr<device> &out_device)
{
    m_profile_log->info("Device " + std::to_string(in_dev_no) + " :: "
                        + std::to_string(in_settings.size()) + " inbound mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_settings.size()); map_no++) {
        std::shared_ptr<map_in> mapping;

        m_profile_log->debug("Device " + std::to_string(in_dev_no) + " :: Read settings for mapping " + std::to_string(map_no));

        try {
            map_type type = read_mapping_type(in_settings[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_type::command:
                    mapping = std::make_shared<map_in_cmd>(xp());
                    break;

                case map_type::dataref:
                    mapping = std::make_shared<map_in_drf>(xp());
                    break;

                case map_type::encoder:
                    mapping = std::make_shared<map_in_enc>(xp());
                    break;

                case map_type::internal:
                    m_profile_log->warn("Device " + std::to_string(in_dev_no) + " :: "
                                        + "Mapping " + std::to_string(map_no) + " :: Mapping type 'int' is currently unsupported");
                    break;

                case map_type::push_pull:
                    mapping = std::make_shared<map_in_pnp>(xp());
                    break;

                case map_type::slider:
                    mapping = std::make_shared<map_in_sld>(xp());
                    break;

                case map_type::none:
                    m_profile_log->error("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Invalid mapping type");
                    break;
            }

            if (mapping == nullptr) {
                m_profile_log->error("Line " + std::to_string(in_settings[map_no].location().line()) + " :: "
                                     + in_settings[map_no].location().line_str());
                m_profile_log->error(" --> Error reading mapping");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(*m_profile_log, in_settings[map_no], *out_device, m_config);

            if (mapping->check(*m_profile_log)) {
                out_device->add_inbound_map(mapping);
            } else {
                m_profile_log->error("Line " + std::to_string(in_settings[map_no].location().line()) + " :: "
                                     + in_settings[map_no].location().line_str());
                m_profile_log->error(" --> Parameters incomplete or incorrect");
            }
        } catch (toml::type_error &error) {
            m_profile_log->error("Line " + std::to_string(in_settings[map_no].location().line()) + " :: "
                                 + in_settings[map_no].location().line_str());
            m_profile_log->error(" --> Error reading config");
            m_profile_log->error(error.what());
        }
    }
}


/**
 * Create the outbound mapping for a device and store it
 */
void profile::create_outbound_mapping(int in_dev_no, toml::array in_settings, const std::shared_ptr<device> &out_device)
{
    m_profile_log->info("Device " + std::to_string(in_dev_no) + " :: "
                        + std::to_string(in_settings.size()) + " outbound mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_settings.size()); map_no++) {
        std::shared_ptr<map_out> mapping;

        m_profile_log->debug("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Reading config");

        try {
            map_type type = read_mapping_type(in_settings[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_type::dataref:
                    mapping = std::make_shared<map_out_drf>(xp());
                    break;

                case map_type::internal:
                    m_profile_log->warn("Device " + std::to_string(in_dev_no) + " :: "
                                        + "Mapping " + std::to_string(map_no) + " :: Mapping type 'int' is currently unsupported");
                    break;

                default:
                    m_profile_log->error("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Invalid mapping type");
                    break;
            }

            if (mapping == nullptr) {
                m_profile_log->error("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Error reading config");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(*m_profile_log, in_settings[map_no]);

            if (mapping->check(*m_profile_log))
                out_device->add_outbound_map(mapping);
            else
                m_profile_log->error("Device " + std::to_string(in_dev_no)+ " :: Mapping " + std::to_string(map_no) + " :: Parameters incomplete or incorrect");

        } catch (toml::type_error &error) {
            m_profile_log->error("Device " + std::to_string(in_dev_no) + " :: Mapping " + std::to_string(map_no) + " :: Error reading mapping");
            m_profile_log->error(error.what());
        }
    }
}


/**
 * Translate a type string to an enum value
 */
map_type profile::translate_map_type(std::string_view in_type_str)
{
    map_type type = map_type::none;

    if (in_type_str == CFG_MAPTYPE_COMMAND)
        type = map_type::command;
    else if (in_type_str == CFG_MAPTYPE_SLIDER)
        type = map_type::slider;
    else if (in_type_str == CFG_MAPTYPE_DATAREF)
        type = map_type::dataref;
    else if (in_type_str == CFG_MAPTYPE_PUSH_PULL)
        type = map_type::push_pull;
    else if (in_type_str == CFG_MAPTYPE_ENCODER)
        type = map_type::encoder;
    else if (in_type_str == CFG_MAPTYPE_INTERNAL)
        type = map_type::internal;

    return type;
}


/**
 * Read the mapping type
 */
map_type profile::read_mapping_type(toml::value &in_settings)
{
    map_type type = map_type::none;

    try {
        // read type
        if (in_settings.contains(CFG_KEY_TYPE)) {
            std::string type_str {in_settings[CFG_KEY_TYPE].as_string()};

            m_profile_log->debug_line(in_settings.location().line(), "Parameter type = '" + type_str + "'");

            // get the mapping type
            type = translate_map_type(type_str);
        } else {
            m_profile_log->error("Line " + std::to_string(in_settings.location().line()) + " :: " + in_settings.location().line_str());
            m_profile_log->error(" --> Parameter '" + std::string(CFG_KEY_TYPE) + "' is missing");
        }
    } catch (toml::type_error &error) {
        m_profile_log->error("Line " + std::to_string(in_settings.location().line()) + " :: " + in_settings.location().line_str());
        m_profile_log->error("Line " + std::to_string(in_settings.location().line()) + " :: Error reading mapping");
        m_profile_log->error(error.what());
    }

    return type;
}

} // Namespace xmidictrl
