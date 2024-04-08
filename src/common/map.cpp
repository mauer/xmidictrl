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

#include "map.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Set the mapping number
 */
void map::set_no(unsigned int in_no)
{
	m_no = in_no;
}


/**
 * Return the mapping number
 */
unsigned int map::no() const
{
	return m_no;
}


/**
 * Set the include name
 */
void map::set_include_name(std::string_view in_name)
{
	m_include_name = in_name;
}


/**
 * Return the include
 */
std::string_view map::include_name() const
{
	return m_include_name;
}


/**
 * Return the chanel number
 */
unsigned char map::channel() const
{
	return m_channel;
}


/**
 * Return the data type
 */
map_data_1_type map::data_1_type() const
{
	return m_data_1_type;
}


/**
 * Return the data 1 number
 */
unsigned char map::data_1() const
{
	return m_data_1;
}


/**
 * Return data 1 as string
 */
std::string map::data_1_as_string() const
{
	std::string str = fmt::format("{} {}", data_1_type_as_string(), std::to_string(m_data_1));
	return str;
}


/**
 * Return the sublayer name
 */
std::string_view map::sl() const
{
	return m_sl;
}


/**
 * Return the source line
 */
std::string_view map::source_line() const
{
	return m_source_line;
}


/**
 * Return the mapping as text
 */
std::string_view map::map_text(bool in_short)
{
	if (in_short) {
		if (m_map_text_short.empty())
			m_map_text_short = build_mapping_text(true);

		return m_map_text_short;
	} else {
		if (m_map_text_long.empty())
			m_map_text_long = build_mapping_text(false);

		return m_map_text_long;
	}
}


/**
 * Return a string containing channel, type and data
 */
std::string map::get_key()
{
	std::string type_code = data_1_type_as_string();
	return build_map_key(m_channel, type_code, m_data_1);
}


/**
 * Check the mapping
 */
bool map::check(text_logger& in_log, const device_settings& in_dev_settings)
{
	// check if a sublayer was defined
	if (!m_sl.empty() && in_dev_settings.sl_dataref.empty()) {
		in_log.error(source_line());
		in_log.error(" --> Sublayer is defined for mapping, but no sublayer dataref was defined in the device");
		return false;
	}

	if (m_channel != MIDI_NONE && m_data_1 != MIDI_NONE && m_data_1_type != map_data_1_type::none)
		return true;
	else
		return false;
}


/**
 * Build an unique key for mappings
 *
 * @param in_ch MIDI Channel
 * @param in_type_code MIDI Type Code
 * @param in_data MIDI Data 1 value
 * @return
 */
std::string map::build_map_key(const unsigned char in_ch, std::string_view in_type_code, const unsigned char in_data)
{
	std::stringstream ss;
	ss << static_cast<int>(in_ch) << in_type_code << static_cast<int>(in_data);

	return ss.str();
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the common config
 */
void map::read_common_config(text_logger& in_log, toml::value& in_data, bool in_read_sl)
{
	// set source line
	m_source_line = fmt::format("Line {} :: {}", in_data.location().line(), utils::trim(in_data.location().line_str()));

	// required config
	read_channel(in_log, in_data);
	read_data_1(in_log, in_data);

	// optional config
	if (in_read_sl)
		read_sublayer(in_log, in_data);
}


/**
 * Check if the mapping is defined for the current sublayer
 */
bool map::check_sublayer(std::string_view in_sl_value)
{
	if (in_sl_value != m_sl && !m_sl.empty())
		return false;

	return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter channel
 */
void map::read_channel(text_logger& in_log, toml::value& in_data)
{
	m_channel = 11; // default channel 11

	try {
		// read channel
		if (in_data.contains(c_cfg_ch.data())) {
			m_channel = static_cast<char>(in_data[c_cfg_ch.data()].as_integer());

			in_log.debug_param(in_data.location().line(), c_cfg_ch, std::to_string(m_channel));
		} else {
			in_log.info(fmt::format(" --> Line {} :: Parameter '{}' is missing, will use default channel '11'",
									in_data.location().line(),
									c_cfg_ch));
			in_log.debug_line(in_data.location().line(),
							  fmt::format("Parameter '{}' = '{}' (Default Value)", c_cfg_ch, m_channel));
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), "Error reading mapping");
		in_log.error(error.what());
	}
}


/**
 * Read parameter data
 */
void map::read_data_1(text_logger& in_log, toml::value& in_data)
{
	m_data_1 = MIDI_NONE;
	m_data_1_type = map_data_1_type::none;

	try {
		// read control change
		if (in_data.contains(c_cfg_cc.data())) {
			m_data_1 = static_cast<char>(in_data[c_cfg_cc.data()].as_integer());
			m_data_1_type = map_data_1_type::control_change;

			in_log.debug_param(in_data.location().line(), c_cfg_cc, std::to_string(m_data_1));
		} else if (in_data.contains(c_cfg_note.data())) {
			m_data_1 = static_cast<char>(in_data[c_cfg_note.data()].as_integer());
			m_data_1_type = map_data_1_type::note;

			in_log.debug_param(in_data.location().line(), c_cfg_note, std::to_string(m_data_1));
		} else if (in_data.contains(c_cfg_pitch.data())) {
			m_data_1 = 0; // fixed value for pitch bend messages
			m_data_1_type = map_data_1_type::pitch_bend;

			in_log.debug_line(in_data.location().line(),
							  fmt::format("Parameter '{}' = '{}' (fixed value for pitch bend)",
										  c_cfg_pitch,
										  std::to_string(m_data_1)));
		} else if (in_data.contains(c_cfg_program_change.data())) {
			m_data_1 = static_cast<char>(in_data[c_cfg_program_change.data()].as_integer());
			m_data_1_type = map_data_1_type::program_change;

			in_log.debug_param(in_data.location().line(), c_cfg_program_change, std::to_string(m_data_1));
		} else {
			in_log.error_line(in_data.location().line(), "Parameter for MIDI type is missing");
		}

	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), "Error reading mapping");
		in_log.error(error.what());
	}
}


/**
 * Read parameter sl
 */
void map::read_sublayer(text_logger& in_log, toml::value& in_data)
{
	m_sl.clear();

	try {
		// read sublayer
		if (in_data.contains(c_cfg_sl.data())) {
			m_sl = in_data[c_cfg_sl.data()].as_string();

			in_log.debug_param(in_data.location().line(), c_cfg_sl, m_sl);
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), "Error reading mapping");
		in_log.error(error.what());
	}
}


/**
 * Return the data 1 type as string
 */
std::string map::data_1_type_as_string() const
{
	std::string str;

	switch (m_data_1_type) {
		using enum map_data_1_type;

		case none:
			str = "";
			break;

		case control_change:
			str = KEY_CONTROL_CHANGE;
			break;

		case note:
			str = KEY_NOTE;
			break;

		case pitch_bend:
			str = KEY_PITCH_BEND;
			break;

		case program_change:
			str = KEY_PROGRAM_CHANGE;
			break;
	}

	return str;
}

} // Namespace xmidictrl