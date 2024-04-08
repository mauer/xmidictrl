//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "device_settings.h"
#include "map_in_enc.h"
#include "midi_device.h"
#include "midi_logger.h"
#include "toml_utils.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile::profile(text_logger& in_text_log, midi_logger& in_midi_log, environment& in_env)
	: m_env(in_env)
	, m_plugin_log(in_text_log)
	, m_midi_log(in_midi_log)
{
	// create my own logger for errors and warnings only
	m_profile_log = std::make_unique<text_logger>(&in_text_log);
	m_profile_log->set_debug_mode(false);
	m_profile_log->set_log_info(false);

	m_device_list = std::make_unique<device_list>(m_env);
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
	m_config = toml::value();
	if (toml_utils::load_file(*m_profile_log, filename, m_config)) {
		m_filename = filename;

		create_device_list();
	} else {
		clear();
		return false;
	}

	// open the midi connections
	if (m_device_list->open_connections()) {
		m_filename = filename;
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
	clear();

	if (!m_filename.empty())
		m_profile_log->debug("File '" + m_filename + "' closed");

	m_config = toml::value();
	m_filename.clear();

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
	return toml::find_or<std::string>(m_config, c_cfg_title.data(), "");
}


/**
 * Returns the version of the current profile
 */
std::string profile::version()
{
	return toml::find_or<std::string>(m_config, c_cfg_version.data(), "");
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
			return m_env.current_aircraft_path().string() + m_env.current_aircraft_icao() + "_"
				   + std::string(FILENAME_PROFILE);

		case filename_prefix::acf_name:
			return m_env.current_aircraft_path().string() + m_env.current_aircraft_acf_name() + "_"
				   + std::string(FILENAME_PROFILE);

		default:
			return m_env.current_aircraft_path().string() + std::string(FILENAME_PROFILE);
	}
}


/**
 * Return the filename for the profile path
 */
std::string profile::get_filename_profiles_path(filename_prefix in_prefix)
{
	switch (in_prefix) {
		case filename_prefix::icao:
			return m_env.profiles_path().string() + m_env.current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

		case filename_prefix::acf_name:
			return m_env.profiles_path().string() + m_env.current_aircraft_acf_name() + "_"
				   + std::string(FILENAME_PROFILE);

		default:
			return m_env.profiles_path().string() + std::string(FILENAME_PROFILE);
	}
}


/**
 * Process all outbound mappings
 */
void profile::process()
{
	if (!m_init_send) {
		// process init mappings
		m_device_list->process_init_mappings();
		m_init_send = true;
	}

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
	// TODO : combine clear and close
	m_filename.clear();

	m_loaded = false;
	m_init_send = false;

	m_config = toml::value();

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
	if (m_env.settings().use_common_profile()) {
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
		device* new_device;

		if (in_is_virtual)
			new_device = m_device_list->create_virtual_device(m_plugin_log, m_midi_log, std::move(settings));
		else
			new_device = m_device_list->create_midi_device(m_plugin_log, m_midi_log, std::move(settings));

		// let's add the mappings
		if (new_device != nullptr) {
			// add mappings from include files
			add_mappings_from_include(in_is_virtual, *new_device);

			m_profile_log->debug(get_log_prefix(in_is_virtual, in_dev_no) + "Add mappings from aircraft profile");

			create_device_mappings(in_params, *new_device);
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
std::unique_ptr<device_settings> profile::create_device_settings(toml::value in_params,
																 bool in_is_virtual,
																 size_t in_dev_no)
{
	std::string dev_name {};

	if (in_is_virtual)
		dev_name.append("virtual device");
	else
		dev_name.append(fmt::format("MIDI device {}", in_dev_no));

	m_profile_log->debug(fmt::format("Read settings for {}", dev_name));

	// read all the required parameters
	auto settings = std::make_unique<device_settings>();

	try {
		// name
		settings->name = toml_utils::read_string(*m_profile_log, in_params, CFG_KEY_NAME);

		if (settings->name.empty()) {
			settings->name = "<undefined>";
			m_profile_log->warn(get_log_prefix(in_is_virtual, in_dev_no) + "Parameter '" + std::string(CFG_KEY_NAME)
								+ "' is missing");
		}

		// include files
		if (toml_utils::contains(*m_profile_log, in_params, c_cfg_include)) {
			if (toml_utils::is_array(*m_profile_log, in_params, c_cfg_include)) {
				settings->include = toml_utils::read_str_set_array(*m_profile_log, in_params, c_cfg_include);
			} else {
				settings->include.clear();
				std::string include_name = toml_utils::read_string(*m_profile_log, in_params, c_cfg_include);

				settings->include.insert(include_name);
			}
		}

		// sl dataref
		if (toml_utils::contains(*m_profile_log, in_params, CFG_KEY_SL_DATAREF))
			settings->sl_dataref = toml_utils::read_string(*m_profile_log, in_params, CFG_KEY_SL_DATAREF);

		if (!settings->sl_dataref.empty()) {
			m_profile_log->info(get_log_prefix(in_is_virtual, in_dev_no) + " Sublayer mode activated");

			if (!m_env.drf().check(settings->sl_dataref)) {
				m_profile_log->error(get_log_prefix(in_is_virtual, in_dev_no) + "Dataref '" + settings->sl_dataref
									 + "' not found");
				return {};
			}
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
			settings->note_mode = device_settings::note_mode_from_code(
				toml_utils::read_string(*m_profile_log, in_params, CFG_KEY_MODE_NOTE));

			// outbound delay
			if (toml_utils::contains(*m_profile_log, in_params, CFG_KEY_OUTBOUND_DELAY))
				settings->outbound_delay = toml_utils::read_float(*m_profile_log,
																  in_params,
																  CFG_KEY_OUTBOUND_DELAY,
																  m_env.settings().default_outbound_delay());
			else
				settings->outbound_delay = m_env.settings().default_outbound_delay();

			// mode outbound
			settings->send_mode = device_settings::send_mode_from_code(
				toml_utils::read_string(*m_profile_log, in_params, CFG_KEY_MODE_OUT));

			// default encoder mode
			settings->default_enc_mode = map_in_enc::encoder_mode_from_code(
				toml_utils::read_string(*m_profile_log, in_params, c_cfg_default_encoder_mode));
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
void profile::add_mappings_from_include(bool in_is_virtual, device& in_device)
{
	if (in_device.settings().include.empty()) {
		if (in_is_virtual)
			m_profile_log->debug("Virtual device :: No includes found");
		else
			m_profile_log->debug("Device " + std::to_string(in_device.settings().device_no) + " :: No includes found");
		return;
	}

	for (auto& inc_name: in_device.settings().include) {
		m_profile_log->debug("Device " + std::to_string(in_device.settings().device_no)
							 + " :: Add mappings from include file '" + inc_name + "'");

		if (inc_name.empty())
			continue;

		// build the filename
		std::string filename = m_env.includes_path().string() + inc_name + INCLUDE_FILE_SUFFIX;

		// load include file
		toml::value inc_file;
		if (!toml_utils::load_file(*m_profile_log, filename, inc_file))
			continue;

		// let's read all the mappings
		create_device_mappings(inc_file, in_device, inc_name);

		m_profile_log->debug("Device " + std::to_string(in_device.settings().device_no)
							 + " :: Finished adding mappings from include file '" + inc_name + "'");
	}
}


/**
 * Create mappings for the device
 */
void profile::create_device_mappings(toml::value in_params, device& in_device, std::string_view in_inc_name)
{
	// create init mappings
	if (in_device.type() == device_type::midi_device && in_params.contains(CFG_KEY_MAPPING_INIT)) {
		auto& midi_dev = dynamic_cast<midi_device&>(in_device);
		midi_dev.mapping_init().create_mappings(*m_profile_log,
												in_params[CFG_KEY_MAPPING_INIT].as_array(),
												midi_dev.settings(),
												in_inc_name);
	} else {
		m_profile_log->info(get_log_prefix_from_device(in_device) + "No init mappings found");
	}

	// create inbound mappings
	if (in_params.contains(CFG_KEY_MAPPING_IN))
		in_device.mapping_in().create_mappings(*m_profile_log,
											   in_params[CFG_KEY_MAPPING_IN].as_array(),
											   m_env,
											   in_device.type() == device_type::virtual_device,
											   in_device.settings(),
											   in_inc_name,
											   m_config);
	else
		m_profile_log->info(get_log_prefix_from_device(in_device) + "No inbound mappings found");


	// create outbound mappings
	if (in_device.type() == device_type::midi_device && in_params.contains(CFG_KEY_MAPPING_OUT)) {
		auto& midi_dev = dynamic_cast<midi_device&>(in_device);
		midi_dev.mapping_out().create_mappings(*m_profile_log,
											   in_params[CFG_KEY_MAPPING_OUT].as_array(),
											   m_env,
											   midi_dev.settings(),
											   in_inc_name);
	} else {
		m_profile_log->info(get_log_prefix_from_device(in_device) + "No outbound mappings found");
	}
}


/**
 * Get the log prefix from a device
 */
std::string profile::get_log_prefix_from_device(device& in_device)
{
	if (in_device.type() == device_type::virtual_device) {
		return get_log_prefix(true);
	} else {
		auto& midi_dev = dynamic_cast<midi_device&>(in_device);
		return get_log_prefix(false, midi_dev.settings().device_no);
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
