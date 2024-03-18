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

#include "map_in_drf.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_drf::map_in_drf(environment& in_env)
	: map_in_label(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_drf::type()
{
	return map_in_type::dataref;
}


/**
 * Read settings from config
 */
void map_in_drf::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
	in_log.debug(" --> Line " + std::to_string(in_data.location().line()) + " :: Read settings for type 'drf'");
	map_in_label::read_config(in_log, in_data, in_config);

	// read mode
	m_mode = dataref_mode_from_code(toml_utils::read_string(in_log, in_data, CFG_KEY_MODE));

	// read dataref
	m_dataref = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF);

	// check if a values array has been defined
	m_values = toml_utils::read_str_vector_array(in_log, in_data, CFG_KEY_VALUES);

	if (m_values.empty()) {
		// read value on
		std::string value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_ON);

		if (!value.empty())
			m_values.push_back(value);

		// read value off
		if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_OFF))
			value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_OFF);

		if (!value.empty())
			m_values.push_back(value);
	}
}


/**
 * Check the mapping
 */
bool map_in_drf::check(text_logger& in_log, const device_settings& in_dev_settings)
{
	bool result = true;

	if (!map::check(in_log, in_dev_settings))
		result = false;

	if (m_dataref.empty()) {
		in_log.error(source_line());
		in_log.error(" --> Parameter '" + std::string(CFG_KEY_DATAREF) + "' is empty");
		result = false;
	}

	if (!env().drf().check(m_dataref)) {
		in_log.error(source_line());
		in_log.error(" --> Dataref '" + m_dataref + "' not found");
		result = false;
	}

	if (m_values.empty()) {
		in_log.error(source_line());
		in_log.error(" --> No values defined");
		result = false;
	}

	if (m_mode == dataref_mode::momentary && m_values.size() != 2) {
		in_log.error(source_line());
		in_log.error(" --> When parameter '" + std::string(CFG_KEY_MODE)
					 + "' is 'momentary', two values (on/off) are expected");
		result = false;
	}

	return result;
}


/**
 * Execute the action in X-Plane
 */
std::unique_ptr<map_result> map_in_drf::execute(map_param* in_param)
{
	auto result = std::make_unique<map_result>();
	result->completed = true;

	auto param_in = get_param_in(in_param);
	if (param_in == nullptr)
		return result;

	if (!check_sublayer(param_in->sl_value()))
		return result;

	// if toggle mode then only process key pressed event
	if (m_mode == dataref_mode::toggle && param_in->msg().data_2() != MIDI_DATA_2_MAX)
		return result;

	if (m_values.size() == 2 && m_mode == dataref_mode::momentary) {
		std::string value;
		if (param_in->msg().data_2() == MIDI_DATA_2_MAX)
			value = m_values[0];
		else
			value = m_values[1];

		// change dataref and display label
		param_in->msg().log().debug(" --> Change dataref '" + m_dataref + "' to value '" + value + "'");

		env().drf().write(param_in->msg().log(), m_dataref, value);
		display_label(param_in->msg().log(), value);
	} else {
		toggle_dataref(param_in->msg().log(), m_dataref, m_values);
	}

	return result;
}


/**
 * Return mapped datarefs
 */
std::string map_in_drf::map_text_cmd_drf()
{
	return m_dataref;
}


/**
 * Return mapped parameter
 */
std::string map_in_drf::map_text_parameter()
{
	std::string map_str {};

	// Values
	if (m_values.size() == 1) {
		map_str.append("Value = " + m_values[0]);
	} else {
		map_str.append("Values = ");

		std::string val_str {};

		for (const auto& value: m_values) {
			if (!val_str.empty())
				val_str.append(", ");

			val_str.append(fmt::format("'{}'", value));
		}

		map_str.append(val_str);
	}

	map_str.append(c_newline);

	if (m_mode == dataref_mode::toggle)
		map_str.append("Mode = 'toggle'");
	else
		map_str.append("Mode = 'momentary'");

	return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_drf::build_mapping_text(bool in_short)
{
	std::string map_str {};
	std::string sep_str {", "};

	if (!in_short) {
		sep_str = "\n";
		map_str = " ====== Dataref ======" + sep_str;

		if (!sl().empty())
			map_str.append("Sublayer = '" + std::string(sl()) + "'" + sep_str);
	}

	map_str.append("Dataref = '" + m_dataref + "'" + sep_str);

	map_str.append("Values = [");

	for (const auto& value: m_values) map_str.append(" '" + value + "', ");

	map_str.append("]" + sep_str);

	if (m_mode == dataref_mode::toggle)
		map_str.append("Mode = 'toggle'");
	else
		map_str.append("Mode = 'momentary'");

	return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the dataref mode for a given string
 */
dataref_mode map_in_drf::dataref_mode_from_code(std::string_view in_mode) const
{
	if (in_mode == "momentary")
		return dataref_mode::momentary;
	else
		return dataref_mode::toggle;
}

} // Namespace xmidictrl