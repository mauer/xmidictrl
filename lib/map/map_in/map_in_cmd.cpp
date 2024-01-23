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

#include "map_in_cmd.h"

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
map_in_cmd::map_in_cmd(environment& in_env)
	: map_in_label(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_cmd::type()
{
	return map_in_type::command;
}


/**
 * Return data 2 on
 */
unsigned char map_in_cmd::data_2_on() const
{
	return m_data_2_on;
}


/**
 * Return data 2 off
 */
unsigned char map_in_cmd::data_2_off() const
{
	return m_data_2_off;
}


/**
 * Read settings from config
 */
void map_in_cmd::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
	in_log.debug_line(in_data.location().line(), "Read settings for type 'cmd'");
	map_in_label::read_config(in_log, in_data, in_config);

	// read the command
	m_command = toml_utils::read_string(in_log, in_data, c_cfg_command);

	// read data_2 parameters
	m_data_2_on = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_on, MIDI_DATA_2_MAX);
	m_data_2_off = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_off, MIDI_DATA_2_MIN);
}


/**
 * Check the mapping
 */
bool map_in_cmd::check(text_logger& in_log, const device_settings& in_dev_settings)
{
	if (!map::check(in_log, in_dev_settings))
		return false;

	if (m_command.empty()) {
		in_log.error(source_line());
		in_log.error(fmt::format(" --> Parameter '{}' is empty", c_cfg_command));
		return false;
	}

	if (m_data_2_on > MIDI_DATA_2_MAX) {
		in_log.error(source_line());
		in_log.error(fmt::format(" --> Invalid value for parameter '{}', it has to be between {} and {}",
								 c_cfg_data_2_on,
								 MIDI_DATA_2_MIN,
								 MIDI_DATA_2_MAX));

		return false;
	}

	if (m_data_2_off > MIDI_DATA_2_MAX) {
		in_log.error(source_line());
		in_log.error(fmt::format(" --> Invalid value for parameter '{}', it has to be between {} and {}",
								 c_cfg_data_2_off,
								 MIDI_DATA_2_MIN,
								 MIDI_DATA_2_MAX));

		return false;
	}

	return true;
}


/**
 * Execute the action in X-Plane
 */
std::unique_ptr<map_result> map_in_cmd::execute(map_param* in_param)
{
	auto result = std::make_unique<map_result>();
	result->completed = true;

	auto param_in = get_param_in(in_param);
	if (param_in == nullptr)
		return result;

	if (!check_sublayer(param_in->sl_value()))
		return result;

	if (param_in->msg().data_2() == m_data_2_on) {
		param_in->msg().log().debug(fmt::format(" --> Begin execution of command '{}'", m_command));
		env().cmd().begin(param_in->msg().log(), m_command);
	} else if (param_in->msg().data_2() == m_data_2_off) {
		param_in->msg().log().debug(fmt::format(" --> End execution of command '{}'", m_command));
		env().cmd().end(param_in->msg().log(), m_command);
	} else {
		param_in->msg().log().error(fmt::format("Invalid MIDI Data 2 value '{}'", param_in->msg().data_2()));
		param_in->msg().log().error(fmt::format(" --> Supported values for the current mapping are '{}' and '{}'", m_data_2_on, m_data_2_off));
	}

	return result;
}


/**
 * Return mapped command
 */
std::string map_in_cmd::map_text_cmd_drf()
{
	return m_command;
}


/**
 * Return mapped parameter
 */
std::string map_in_cmd::map_text_parameter()
{
	std::string map_str {};

	// Data 2 on/off
	if (m_data_2_on != MIDI_DATA_2_MAX)
		map_str.append(fmt::format("Data 2 on = {}", m_data_2_on));

	if (m_data_2_off != MIDI_DATA_2_MIN) {
		if (!map_str.empty())
			map_str.append(c_newline);

		map_str.append(fmt::format("Data 2 off = {}", m_data_2_off));
	}

	return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_cmd::build_mapping_text(bool in_short)
{
	std::string map_str {};
	std::string sep_str {", "};

	if (!in_short) {
		sep_str = "\n";
		map_str = " ====== Command ======" + sep_str;

		if (!sl().empty())
			map_str.append(fmt::format("Sublayer = '{}'{}", sl(), sep_str));
	}

	if (!in_short)
		map_str.append("Command: ");

	map_str.append(m_command);

	if (m_data_2_on != MIDI_DATA_2_MAX)
		map_str.append(fmt::format("{}Data 2: {}", sep_str, m_data_2_on));

	if (m_data_2_off != MIDI_DATA_2_MIN)
		map_str.append(fmt::format("{}Data 2: {}", sep_str, m_data_2_off));

	return map_str;
}

} // Namespace xmidictrl