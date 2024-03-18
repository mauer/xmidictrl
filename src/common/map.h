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

#ifndef XMC_MAP_H
#define XMC_MAP_H

// Standard
#include <memory>

// toml11
#include "toml.hpp"

// XMidiCtrl
#include "device_settings.h"
#include "map_param.h"
#include "map_result.h"
#include "midi_message.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

// Mapping data type
enum class map_data_1_type
{
	none,
	control_change,
	note,
	pitch_bend,
	program_change
};




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map {
public:
	explicit map() = default;
	virtual ~map() = default;

	void set_no(unsigned int in_no);
	[[nodiscard]] unsigned int no() const;

	void set_include_name(std::string_view in_name);
	[[nodiscard]] std::string_view include_name() const;

	[[nodiscard]] unsigned char channel() const;
	[[nodiscard]] map_data_1_type data_1_type() const;
	[[nodiscard]] unsigned char data_1() const;

	std::string data_1_as_string() const;

	[[nodiscard]] std::string_view sl() const;

	[[nodiscard]] std::string_view source_line() const;

	std::string_view map_text(bool in_short = false);

	std::string get_key();

	virtual bool check(text_logger& in_log, const device_settings& in_dev_settings);

	static std::string build_map_key(unsigned char in_ch, std::string_view in_type_code, unsigned char in_data);

	virtual std::unique_ptr<map_result> execute(map_param* in_param) = 0;

protected:
	// constants
	static constexpr std::string_view c_newline {"\n"};

	void read_common_config(text_logger& in_log, toml::value& in_data, bool in_read_sl = true);

	bool check_sublayer(std::string_view in_sl_value);

	virtual std::string build_mapping_text(bool in_short) = 0;

private:
	void read_channel(text_logger& in_log, toml::value& in_data);
	void read_data_1(text_logger& in_log, toml::value& in_data);
	void read_sublayer(text_logger& in_log, toml::value& in_data);

	[[nodiscard]] std::string data_1_type_as_string() const;

	static constexpr std::string_view c_cfg_ch {"ch"};
	static constexpr std::string_view c_cfg_cc {"cc"};
	static constexpr std::string_view c_cfg_note {"note"};
	static constexpr std::string_view c_cfg_sl {"sl"};

	unsigned int m_no {0};
	std::string m_include_name {};

	char m_channel {MIDI_NONE};

	map_data_1_type m_data_1_type {map_data_1_type::none};
	char m_data_1 {MIDI_NONE};

	std::string m_sl {};

	std::string m_source_line {};

	std::string m_map_text_long {};
	std::string m_map_text_short {};
};

} // Namespace xmidictrl

#endif // XMC_MAP_H
