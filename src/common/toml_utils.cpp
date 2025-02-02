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

#include "toml_utils.h"

// fmt
#include "fmt/format.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Load a toml config from a given file
 */
bool toml_utils::load_file(text_logger& in_log, std::string_view in_filename, toml::value& out_config)
{
	// check file name
	if (in_filename.empty()) {
		in_log.error("Cannot load file, because the given filename is empty");
		return false;
	}

	// check if file exists
	if (!std::filesystem::exists(in_filename)) {
		in_log.error("File '" + std::string(in_filename) + "' not found!");
		return false;
	}

	try {
		// load config file
		out_config = toml::parse(in_filename.data());
		in_log.debug("File '" + std::string(in_filename) + "' loaded successfully");
	} catch (const toml::syntax_error& error) {
		in_log.error("Error parsing file '" + std::string(in_filename) + "'");
		in_log.error(error.what());
		return false;

	} catch (const std::runtime_error& error) {
		in_log.error("Error opening file '" + std::string(in_filename) + "'");
		in_log.error(error.what());
		return false;
	}

	return true;
}


/**
 * Check if the given key is in the config
 */
bool toml_utils::contains(text_logger& in_log, toml::value& in_data, std::string_view in_name)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::contains --> name is empty)");
		return false;
	}

	try {
		if (in_data.contains(in_name.data()))
			return true;
		else
			return false;
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return false;
}


/**
 * Check if the given key is an array
 */
bool toml_utils::is_array(text_logger& in_log, toml::value& in_data, std::string_view in_name)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::is_array --> name is empty)");
		return false;
	}

	try {
		// read dataref
		if (in_data.contains(in_name.data())) {
			if (in_data[in_name.data()].is_array())
				return true;
			else
				return false;
		} else {
			return false;
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return false;
}

/**
 * Read a boolean value
 */
bool toml_utils::read_bool(text_logger& in_log, toml::value& in_data, std::string_view in_name, bool in_fallback)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_bool --> name is empty)");
		return in_fallback;
	}

	bool value = in_fallback;

	try {
		// read dataref
		if (contains(in_log, in_data, in_name)) {
			if (in_data[in_name.data()].is_boolean()) {
				value = in_data[in_name.data()].as_boolean();
				in_log.debug_param(in_data.location().line(), in_name, std::to_string(value));
			} else {
				in_log.error_line(in_data.location().line(), in_data.location().line_str());
				in_log.error(" --> Parameter '" + std::string(in_name) + "' is not a boolean");
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return value;
}

/**
 * Read the value of a string parameter
 */
std::string toml_utils::read_string(text_logger& in_log, toml::value& in_data, std::string_view in_name)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_string --> name is empty)");
		return {};
	}

	std::string value {};

	try {
		// read dataref
		if (contains(in_log, in_data, in_name)) {
			value = in_data[in_name.data()].as_string();
			in_log.debug_param(in_data.location().line(), std::string(in_name), value);
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return value;
}


/**
 * Read the values of a string array and return as set
 */
std::set<std::string> toml_utils::read_str_set_array(text_logger& in_log,
													 toml::value& in_data,
													 std::string_view in_name)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_str_set_array --> name is empty)");
		return {};
	}

	std::set<std::string> list {};

	try {
		// read dataref array
		if (contains(in_log, in_data, in_name) && (in_data[in_name.data()].is_array())) {
			for (int i = 0; i < in_data[in_name.data()].size(); i++) {
				std::string value = in_data[in_name.data()][i].as_string();

				in_log.debug_param(in_data.location().line(), in_name, value);

				if (!value.empty())
					list.insert(value.data());
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return list;
}


/**
 * Read the values of a string array and return as vector
 */
std::vector<std::string> toml_utils::read_str_vector_array(text_logger& in_log,
														   toml::value& in_data,
														   std::string_view in_name)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_str_vector_array --> name is empty)");
		return {};
	}

	std::vector<std::string> list;

	try {
		// read array
		if (contains(in_log, in_data, in_name) && (in_data[in_name.data()].is_array())) {
			for (int i = 0; i < in_data[in_name.data()].size(); i++) {
				std::string value = in_data[in_name.data()][i].as_string();

				in_log.debug_param(in_data.location().line(), in_name, value);

				if (!value.empty())
					list.push_back(value);
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return list;
}


/**
 * Read the values of a key/value array and return as map
 */
std::map<std::string, std::string> toml_utils::read_str_map_array(text_logger& in_log,
																  toml::value& in_data,
																  std::string_view in_name,
																  std::string_view in_key_name,
																  std::string_view in_value_name)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_str_map_array --> name is empty)");
		return {};
	}

	std::map<std::string, std::string> list;

	try {
		// read array
		if (contains(in_log, in_data, in_name) && (in_data[in_name.data()].is_array())) {
			for (int i = 0; i < in_data[in_name.data()].size(); i++) {
				auto pairs = in_data[in_name.data()][i];

				auto key = toml_utils::read_string(in_log, pairs, in_key_name);
				auto value = toml_utils::read_string(in_log, pairs, in_value_name);

				if (!value.empty())
					list.try_emplace(key, value);
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return list;
}


/**
 * Read a MIDI value (0..127)
 */
char toml_utils::read_midi_value(text_logger& in_log, toml::value& in_data, std::string_view in_name, char in_fallback)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_midi_value --> name is empty)");
		return in_fallback;
	}

	char value = in_fallback;

	try {
		// read dataref
		if (contains(in_log, in_data, in_name)) {
			if (in_data[in_name.data()].is_integer()) {
				auto int_value = static_cast<int>(in_data[in_name.data()].as_integer());
				in_log.debug_param(in_data.location().line(), in_name, std::to_string(value));

				if (int_value >= MIDI_DATA_2_MIN && int_value <= MIDI_DATA_2_MAX) {
					value = static_cast<char>(int_value);
				} else {
					in_log.error_line(in_data.location().line(), in_data.location().line_str());
					in_log.error(fmt::format(" --> Parameter '{}' is not between {} and {}",
											 in_name,
											 MIDI_DATA_2_MIN,
											 MIDI_DATA_2_MAX));
				}
			} else {
				in_log.error_line(in_data.location().line(), in_data.location().line_str());
				in_log.error(fmt::format(" --> Parameter '{}' is not numeric", in_name));
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return value;
}


/**
 * Read the value of an integer
 */
int toml_utils::read_int(text_logger& in_log, toml::value& in_data, std::string_view in_name, int in_fallback)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_int --> name is empty)");
		return in_fallback;
	}

	int value = in_fallback;

	try {
		// read dataref
		if (contains(in_log, in_data, in_name)) {
			if (in_data[in_name.data()].is_integer()) {
				value = static_cast<int>(in_data[in_name.data()].as_integer());

				in_log.debug_param(in_data.location().line(), in_name, std::to_string(value));
			} else {
				in_log.error_line(in_data.location().line(), in_data.location().line_str());
				in_log.error(" --> Parameter '" + std::string(in_name) + "' is not numeric");
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return value;
}


/**
 * Read the value of a float
 */
float toml_utils::read_float(text_logger& in_log, toml::value& in_data, std::string_view in_name, float in_fallback)
{
	if (in_name.empty()) {
		in_log.error("Internal error (toml_utils::read_float --> name is empty)");
		return in_fallback;
	}

	float value = in_fallback;

	try {
		// read dataref
		if (contains(in_log, in_data, in_name)) {
			if (in_data[in_name.data()].is_floating()) {
				value = static_cast<float>(in_data[in_name.data()].as_floating());

				in_log.debug_param(in_data.location().line(), in_name, fmt::format("{:.2f}", value));
			} else if (in_data[in_name.data()].is_integer()) {
				value = static_cast<float>(in_data[in_name.data()].as_integer());

				in_log.debug_param(in_data.location().line(), in_name, fmt::format("{:.2f}", value));
			} else {
				in_log.error_line(in_data.location().line(), in_data.location().line_str());
				in_log.error(" --> Parameter '" + std::string(in_name) + "' is not numeric");
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), in_data.location().line_str());
		in_log.error(" --> Error reading mapping");
		in_log.error(error.what());
	}

	return value;
}

} // Namespace xmidictrl
