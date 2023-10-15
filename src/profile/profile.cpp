//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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
#include "device_settings.h"
#include "map_in_cmd.h"
#include "map_in_drf.h"
#include "map_in_enc.h"
#include "map_in_pnp.h"
#include "map_in_sld.h"
#include "map_out_con.h"
#include "map_out_drf.h"
#include "map_out_sld.h"
#include "midi_device.h"
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
profile::profile(text_logger& in_text_log, midi_logger& in_midi_log, environment& in_env, settings& in_settings)
    : config(in_env),
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

    m_device_list.reset();
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
    auto filename = find_profile();

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

            if (!env().drf().check(m_sl_dataref)) {
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
text_logger& profile::log()
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
 * Return the list of all device
 */
device_list& profile::devices()
{
    return *m_device_list;
}


/**
 * Return the filename for the aircraft path
 */
std::string profile::get_filename_aircraft_path(filename_prefix in_prefix)
{
    switch (in_prefix) {
        case filename_prefix::icao:
            return env().current_aircraft_path().string() + env().current_aircraft_icao() + "_"
                   + std::string(FILENAME_PROFILE);

        case filename_prefix::acf_name:
            return env().current_aircraft_path().string() + env().current_aircraft_acf_name() + "_"
                   + std::string(FILENAME_PROFILE);

        default:
            return env().current_aircraft_path().string() + std::string(FILENAME_PROFILE);
    }
}


/**
 * Return the filename for the profile path
 */
std::string profile::get_filename_profiles_path(filename_prefix in_prefix)
{
    switch (in_prefix) {
        case filename_prefix::icao:
            return env().profiles_path().string() + env().current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

        case filename_prefix::acf_name:
            return env().profiles_path().string() + env().current_aircraft_acf_name() + "_"
                   + std::string(FILENAME_PROFILE);

        default:
            return env().profiles_path().string() + std::string(FILENAME_PROFILE);
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
void profile::process(text_logger& in_log)
{
    if (!m_init_send) {
        // process init mappings
        m_device_list->process_init_mappings();
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
    // check if there is a profile in the aircraft directory
    auto filename = get_filename_aircraft_path(filename_prefix::none);

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

    // create the virtual device (if specified)
    create_virtual_device();

    try {
        // get all MIDI devices from the profile
        auto profile_dev = toml::find<std::vector<toml::table>>(m_config, CFG_KEY_DEVICE);

        m_profile_log->info(std::to_string(profile_dev.size()) + " MIDI device(s) found in aircraft profile");

        // parse every device
        for (size_t dev_no = 0; dev_no < profile_dev.size(); dev_no++)
            create_device(profile_dev[dev_no], false, dev_no);

    } catch (std::out_of_range& error) {
        m_profile_log->warn("No MIDI devices found in aircraft profile");
        m_profile_log->warn(error.what());
    }
}


/**
 * Create the virtual device
 */
void profile::create_virtual_device()
{
    // do we have a virtual device?
    try {
        auto params = toml::find(m_config, CFG_KEY_VIRTUAL_DEVICE);
        m_profile_log->info("Virtual device found in aircraft profile");

        create_device(params, true);
    } catch (std::out_of_range&) {
        m_profile_log->debug("No virtual device found in aircraft profile");
    }
}


/**
 * Create a new device in the profile
 */
void profile::create_device(const toml::value& in_params, bool in_is_virtual, size_t in_dev_no)
{
    // read all the required parameters
    auto settings = create_device_settings(in_params, in_is_virtual, in_dev_no);

    if (settings == nullptr)
        return;

    try {
        // create the device
        std::shared_ptr<device> new_device;

        if (in_is_virtual)
            new_device = m_device_list->create_virtual_device(m_plugin_log, m_midi_log, settings);
        else
            new_device = m_device_list->create_midi_device(m_plugin_log, m_midi_log, settings);

        // let's add the mappings
        if (new_device != nullptr) {
            // add mappings from include files
            add_mappings_from_include(new_device);

            m_profile_log->debug(get_log_prefix(in_is_virtual, in_dev_no) + "Add mappings from aircraft profile");

            create_device_mappings(in_params, new_device);
        }
    } catch (const std::out_of_range& error) {
        m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Error reading profile");
        m_profile_log->error(error.what());
    } catch (toml::type_error& error) {
        m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Error reading profile");
        m_profile_log->error(error.what());
    }
}


/**
 * Read the device parameters
 */
std::shared_ptr<device_settings> profile::create_device_settings(toml::value in_params,
                                                                 bool in_is_virtual,
                                                                 size_t in_dev_no)
{
    if (in_is_virtual)
        m_profile_log->debug("Read settings for virtual device");
    else
        m_profile_log->debug("Read settings for MIDI device " + std::to_string(in_dev_no));

    // read all the required parameters
    auto settings = std::make_shared<device_settings>();

    try {
        // name
        settings->name = toml_utils::read_string(*m_profile_log, in_params, CFG_KEY_NAME, false);

        if (settings->name.empty()) {
            settings->name = "<undefined>";
            m_profile_log->warn(get_log_prefix(in_is_virtual, in_dev_no)
                                + "Parameter '" + std::string(CFG_KEY_NAME) + "' is missing");
        }

        // include files
        if (toml_utils::is_array(*m_profile_log, in_params, CFG_KEY_INCLUDE)) {
            settings->include = toml_utils::read_str_set_array(*m_profile_log, in_params, CFG_KEY_INCLUDE, false);
        } else {
            settings->include.clear();
            std::string include_name = toml_utils::read_string(*m_profile_log, in_params, CFG_KEY_INCLUDE, false);

            settings->include.insert(include_name);
        }

        // the following parameters are necessary for MIDI devices only
        if (!in_is_virtual) {
            // device number
            settings->device_no = static_cast<int>(in_dev_no);

            // port in
            settings->port_in = toml_utils::read_int(*m_profile_log, in_params, CFG_KEY_PORT_IN);

            if (settings->port_in < 0) {
                m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Invalid inbound port '"
                                     + std::to_string(settings->port_in) + "'");
                return {};
            }

            // port out
            settings->port_out = toml_utils::read_int(*m_profile_log, in_params, CFG_KEY_PORT_OUT);

            if (settings->port_out < 0) {
                m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Invalid outbound port '"
                                     + std::to_string(settings->port_out) + "'");
                return {};
            }

            // mode note
            settings->note_mode = conversions::note_mode_from_code(toml_utils::read_string(*m_profile_log,
                                                                                           in_params,
                                                                                           CFG_KEY_MODE_NOTE,
                                                                                           false));

            // outbound delay
            if (toml_utils::contains(*m_profile_log, in_params, CFG_KEY_OUTBOUND_DELAY, false))
                settings->outbound_delay = toml_utils::read_float(*m_profile_log,
                                                                  in_params,
                                                                  CFG_KEY_OUTBOUND_DELAY,
                                                                  false,
                                                                  m_settings.default_outbound_delay());
            else
                settings->outbound_delay = m_settings.default_outbound_delay();

            // mode outbound
            settings->send_mode = conversions::send_mode_from_code(toml_utils::read_string(*m_profile_log,
                                                                                           in_params,
                                                                                           CFG_KEY_MODE_OUT,
                                                                                           false));

            // default encoder mode
            settings->default_enc_mode = conversions::encoder_mode_from_code(toml_utils::read_string(*m_profile_log,
                                                                                                     in_params,
                                                                                                     CFG_KEY_ENCODER_MODE,
                                                                                                     false));
        }
    } catch (const std::out_of_range& error) {
        m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Error reading profile");
        m_profile_log->error(error.what());
    } catch (toml::type_error& error) {
        m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Error reading profile");
        m_profile_log->error(error.what());
    }

    return settings;
}


/**
 * Add mappings from the specified include files
 */
void profile::add_mappings_from_include(const std::shared_ptr<device>& in_device)
{
    if (in_device->settings().include.empty()) {
        m_profile_log->debug("Device " + std::to_string(in_device->settings().device_no) + " :: No includes found");
        return;
    }

    for (auto& inc_name: in_device->settings().include) {
        m_profile_log->debug("Device " + std::to_string(in_device->settings().device_no)
                             + " ::  Add mappings from include file '" + inc_name + "'");

        if (inc_name.empty())
            continue;

        // build the filename
        std::string filename = env().includes_path().string() + inc_name + INCLUDE_FILE_SUFFIX;

        // load include file
        toml::value inc_file;
        if (!toml_utils::load_file(*m_profile_log, filename, inc_file))
            continue;

        // let's read all the mappings
        create_device_mappings(inc_file, in_device);

        m_profile_log->debug("Device " + std::to_string(in_device->settings().device_no)
                             + " ::  Finished adding mappings from include file '" + inc_name + "'");
    };
}


/**
 * Create mappings for the device
 */
void profile::create_device_mappings(toml::value in_params, const std::shared_ptr<device>& in_device)
{
    // create init mappings
    if (in_device->type() == device_type::midi_device && in_params.contains(CFG_KEY_MAPPING_INIT))
        create_init_mapping(in_params[CFG_KEY_MAPPING_INIT].as_array(),
                            std::static_pointer_cast<midi_device>(in_device));
    else
        m_profile_log->info(get_log_prefix_from_device(in_device) + "No init mappings found");

    // create inbound mappings
    if (in_params.contains(CFG_KEY_MAPPING_IN))
        create_inbound_mapping(in_params[CFG_KEY_MAPPING_IN].as_array(), in_device);
    else
        m_profile_log->info(get_log_prefix_from_device(in_device) + "No inbound mappings found");


    // create outbound mappings
    if (in_device->type() == device_type::midi_device && in_params.contains(CFG_KEY_MAPPING_OUT))
        create_outbound_mapping(in_params[CFG_KEY_MAPPING_OUT].as_array(),
                                std::static_pointer_cast<midi_device>(in_device));
    else
        m_profile_log->info(get_log_prefix_from_device(in_device) + "No outbound mappings found");
}


/**
 * Create the init mapping for a device and store it
 */
void profile::create_init_mapping(toml::array in_settings,
                                  const std::shared_ptr<midi_device>& in_device)
{
    auto dev_no = std::to_string(in_device->settings().device_no);

    m_profile_log->info("Device " + dev_no + " :: "
                        + std::to_string(in_settings.size()) + " init mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_settings.size()); map_no++) {
        std::shared_ptr<map_init> mapping;

        m_profile_log->debug(
            "Device " + dev_no + " :: Mapping " + std::to_string(map_no)
            + " :: Reading config");

        try {
            mapping = std::make_shared<map_init>(env());

            // read the settings and check if everything we need was defined
            mapping->read_config(*m_profile_log, in_settings[map_no]);

            if (mapping->check(*m_profile_log)) {
                in_device->add_init_map(mapping);
                m_profile_log->debug(" --> Mapping added");
            } else {
                m_profile_log->error(
                    "Device " + dev_no + " :: Mapping " + std::to_string(map_no)
                    + " :: Parameters incomplete or incorrect");
            }
        } catch (toml::type_error& error) {
            m_profile_log->error("Device " + dev_no + " :: Mapping " + std::to_string(map_no)
                                 + " :: Error reading mapping");
            m_profile_log->error(error.what());
        }
    }
}


/**
 * Create the inbound mapping for a device and store it
 */
void profile::create_inbound_mapping(toml::array in_settings, const std::shared_ptr<device>& in_device)
{
    auto log_prefix = get_log_prefix_from_device(in_device);

    m_profile_log->info(log_prefix + std::to_string(in_settings.size()) + " inbound mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_settings.size()); map_no++) {
        std::shared_ptr<map_in> mapping;

        m_profile_log->debug(log_prefix + "Read settings for mapping " + std::to_string(map_no));

        try {
            auto type = read_mapping_type(in_settings[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_type::command:
                    mapping = std::make_shared<map_in_cmd>(env());
                    break;

                case map_type::dataref:
                    mapping = std::make_shared<map_in_drf>(env());
                    break;

                case map_type::push_pull:
                    mapping = std::make_shared<map_in_pnp>(env());
                    break;

                case map_type::encoder:
                    if (in_device->type() == device_type::midi_device) {
                        auto dev = std::static_pointer_cast<midi_device>(in_device);
                        mapping = std::make_shared<map_in_enc>(env(), dev->settings().default_enc_mode);
                    } else {
                        m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no)
                                             + " :: mapping type not supported for virtual devices");
                    }
                    break;

                case map_type::slider:
                    if (in_device->type() == device_type::midi_device) {
                        mapping = std::make_shared<map_in_sld>(env());
                    } else {
                        m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no)
                                             + " :: mapping type not supported for virtual devices");
                    }
                    break;

                case map_type::constant:
                case map_type::none:
                    m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no)
                                         + " :: Invalid mapping type");
                    break;
            }

            if (mapping == nullptr) {
                m_profile_log->error("Line " + std::to_string(in_settings[map_no].location().line()) + " :: "
                                     + in_settings[map_no].location().line_str());
                m_profile_log->error(" --> Error reading mapping");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(*m_profile_log, in_settings[map_no], m_config);

            if (mapping->check(*m_profile_log)) {
                in_device->add_inbound_map(mapping);
                m_profile_log->debug(" --> Mapping added");
            } else {
                m_profile_log->error("Line " + std::to_string(in_settings[map_no].location().line()) + " :: "
                                     + in_settings[map_no].location().line_str());
                m_profile_log->error(" --> Parameters incomplete or incorrect");
            }
        } catch (toml::type_error& error) {
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
void profile::create_outbound_mapping(toml::array in_params, const std::shared_ptr<midi_device>& in_device)
{
    auto log_prefix = get_log_prefix_from_device(in_device);

    m_profile_log->info(log_prefix + std::to_string(in_params.size()) + " outbound mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_params.size()); map_no++) {
        std::shared_ptr<map_out> mapping;

        m_profile_log->debug(log_prefix + "Mapping " + std::to_string(map_no) + " :: Reading config");

        try {
            map_type type = read_mapping_type(in_params[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_type::constant:
                    mapping = std::make_shared<map_out_con>(env());
                    break;

                case map_type::dataref:
                    mapping = std::make_shared<map_out_drf>(env());
                    break;

                case map_type::slider:
                    mapping = std::make_shared<map_out_sld>(env());
                    break;

                default:
                    m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no)
                                         + " :: Invalid mapping type");
                    break;
            }

            if (mapping == nullptr) {
                m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no) + " :: Error reading config");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(*m_profile_log, in_params[map_no]);

            if (mapping->check(*m_profile_log)) {
                in_device->add_outbound_map(mapping);
                m_profile_log->debug(" --> Mapping added");
            } else {
                m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no)
                                     + " :: Parameters incomplete or incorrect");
            }
        } catch (toml::type_error& error) {
            m_profile_log->error(log_prefix + "Mapping " + std::to_string(map_no)
                                 + " :: Error reading mapping");
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
    else if (in_type_str == CFG_MAPTYPE_CONSTANT)
        type = map_type::constant;
    else if (in_type_str == CFG_MAPTYPE_SLIDER)
        type = map_type::slider;
    else if (in_type_str == CFG_MAPTYPE_DATAREF)
        type = map_type::dataref;
    else if (in_type_str == CFG_MAPTYPE_PUSH_PULL)
        type = map_type::push_pull;
    else if (in_type_str == CFG_MAPTYPE_ENCODER)
        type = map_type::encoder;

    return type;
}


/**
 * Read the mapping type
 */
map_type profile::read_mapping_type(toml::value& in_params)
{
    map_type type = map_type::none;

    try {
        // read type
        if (in_params.contains(CFG_KEY_TYPE)) {
            std::string type_str {in_params[CFG_KEY_TYPE].as_string()};

            m_profile_log->debug_line(in_params.location().line(), "Parameter type = '" + type_str + "'");

            // get the mapping type
            type = translate_map_type(type_str);
        } else {
            m_profile_log->error(
                "Line " + std::to_string(in_params.location().line()) + " :: " + in_params.location().line_str());
            m_profile_log->error(" --> Parameter '" + std::string(CFG_KEY_TYPE) + "' is missing");
        }
    } catch (toml::type_error& error) {
        m_profile_log->error(
            "Line " + std::to_string(in_params.location().line()) + " :: " + in_params.location().line_str());
        m_profile_log->error("Line " + std::to_string(in_params.location().line()) + " :: Error reading mapping");
        m_profile_log->error(error.what());
    }

    return type;
}


/**
 * Get the log prefix from a device
 */
std::string profile::get_log_prefix_from_device(const std::shared_ptr<device>& in_device)
{
    if (in_device->type() == device_type::virtual_device) {
        return get_log_prefix(true);
    } else {
        auto midi_dev = std::static_pointer_cast<midi_device>(in_device);
        return get_log_prefix(false, midi_dev->settings().device_no);
    }
}


/**
 * Get the log prefix
 */
std::string profile::get_log_prefix(bool in_is_virtual, size_t in_dev_no)
{
    // let's prepare some logging stuff
    std::string log_prefix;
    if (in_is_virtual)
        log_prefix = "Virtual device :: ";
    else
        log_prefix = "Device " + std::to_string(in_dev_no) + " :: ";

    return log_prefix;
}

} // Namespace xmidictrl
