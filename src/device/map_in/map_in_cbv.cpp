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

#include "map_in_cbv.h"

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
map_in_cbv::map_in_cbv(environment& in_env)
	: map_in(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_cbv::type()
{
	return map_in_type::command_by_value;
}


/**
 * Read settings from config
 */
void map_in_cbv::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
	in_log.debug(" --> Line " + std::to_string(in_data.location().line()) + " :: Read settings for type 'cbv'");
	map_in::read_config(in_log, in_data, in_config);

	// read dataref
	m_dataref = toml_utils::read_string(in_log, in_data, c_cfg_dataref);

	// check if a values array has been defined
	m_values = toml_utils::read_str_map_array(in_log, in_data, c_cfg_values, c_cfg_value, c_cfg_command);

	in_log.debug(fmt::format("Values found: {}", m_values.size()));
}


/**
 * Check the mapping
 */
bool map_in_cbv::check(text_logger& in_log, const midi_device_settings& in_dev_settings)
{
	bool result = true;

	if (!map::check(in_log, in_dev_settings))
		result = false;

	if (m_dataref.empty()) {
		in_log.error(source_line());
		in_log.error(fmt::format(" --> Parameter '{}' is empty", c_cfg_dataref));
		result = false;
	}

	if (!env().drf().check(m_dataref)) {
		in_log.error(source_line());
		in_log.error(fmt::format(" --> Dataref '{}' not found", m_dataref));
		result = false;
	}

	if (m_values.empty()) {
		in_log.error(source_line());
		in_log.error(" --> No values defined");
		result = false;
	}

	return result;
}


/**
 * Execute the action in X-Plane
 */
std::unique_ptr<map_result> map_in_cbv::execute(map_param* in_param)
{
	auto result = std::make_unique<map_result>();
	result->completed = true;

	auto param_in = get_param_in(in_param);
	if (param_in == nullptr)
		return result;

	if (!check_sublayer(param_in->sl_value()))
		return result;

	// get current dataref value
	std::string value;
	if (!env().drf().read(param_in->msg().log(), m_dataref, value))
		return result;

	param_in->msg().log().debug(fmt::format(" --> Search for dataref value '{}' in values list", value));

	// look for current value in values list
	if (m_values.contains(value)) {
		std::string cmd = m_values[value];

		param_in->msg().log().debug(fmt::format(" --> Command '{}' found for value '{}'", cmd, value));

		env().cmd().execute(param_in->msg().log(), cmd);
	}

	return result;
}


/**
 * Return the label id
 */
std::string map_in_cbv::map_text_label()
{
	return {};
}


/**
 * Return mapped datarefs
 */
std::string map_in_cbv::map_text_cmd_drf()
{
	return m_dataref;
}


/**
 * Return mapped parameter
 */
std::string map_in_cbv::map_text_parameter()
{
	std::string map_str {};

	// Values
	map_str.append("Values = ");

	std::string val_str {};

	for (const auto& value: m_values) {
		if (!val_str.empty())
			val_str.append(", ");

		val_str.append(fmt::format("Value = '{}' --> Command = '{}''", value.first, value.second));
	}

	map_str.append(val_str);

	return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_cbv::build_mapping_text(bool in_short)
{
	std::string map_str {};
	std::string sep_str {", "};

	if (!in_short) {
		sep_str = "\n";
		map_str = " ====== Command by Value ======" + sep_str;

		if (!sl().empty())
			map_str.append("Sublayer = '" + std::string(sl()) + "'" + sep_str);
	}

	map_str.append("Dataref = '" + m_dataref + "'" + sep_str);

	map_str.append("Values = [");

	for (const auto& value: m_values)
		map_str.append(fmt::format("value = '{}'  cmd = '{}', ", value.first, value.second));

	map_str.append("]" + sep_str);

	return map_str;
}

} // Namespace xmidictrl