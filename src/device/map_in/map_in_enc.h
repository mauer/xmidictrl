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

#ifndef XMC_MAP_IN_ENC_H
#define XMC_MAP_IN_ENC_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "label.h"
#include "map_in.h"
#include "midi_device_settings.h"
#include "midi_message.h"
#include "text_logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in_enc : public map_in {
public:
	explicit map_in_enc(environment& in_env, encoder_mode in_default_enc_mode);
	~map_in_enc() override = default;

	map_in_type type() override;

	void read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config) override;
	bool check(text_logger& in_log, const midi_device_settings& in_dev_settings) override;

	std::unique_ptr<map_result> execute(map_param* in_param) override;

	std::string map_text_label() override;
	std::string map_text_cmd_drf() override;
	std::string map_text_parameter() override;

	static encoder_mode encoder_mode_from_code(std::string_view in_mode);

protected:
	std::string build_mapping_text(bool in_short) override;

private:
	// constants

	static constexpr std::string_view c_cfg_command_down {"command_down"};
	static constexpr std::string_view c_cfg_command_fast_down {"command_fast_down"};

	static constexpr std::string_view c_cfg_command_up {"command_up"};
	static constexpr std::string_view c_cfg_command_fast_up {"command_fast_up"};

	static constexpr std::string_view c_cfg_dataref {"dataref"};

	static constexpr std::string_view c_cfg_data_2_up {"data_2_up"};
	static constexpr std::string_view c_cfg_data_2_down {"data_2_down"};

	static constexpr std::string_view c_cfg_data_2_min {"data_2_min"};
	static constexpr std::string_view c_cfg_data_2_max {"data_2_max"};

	static constexpr std::string_view c_cfg_delay {"delay"};

	static constexpr std::string_view c_cfg_mode {"mode"};

	static constexpr std::string_view c_cfg_modifier_down {"modifier_down"};
	static constexpr std::string_view c_cfg_modifier_fast_down {"modifier_fast_down"};

	static constexpr std::string_view c_cfg_modifier_up {"modifier_up"};
	static constexpr std::string_view c_cfg_modifier_fast_up {"modifier_fest_up"};

	static constexpr std::string_view c_cfg_value_min {"value_min"};
	static constexpr std::string_view c_cfg_value_max {"value_max"};

	static constexpr std::string_view c_cfg_value_wrap {"value_wrap"};

	// enumerations
	enum class encoder_map_type
	{
		dataref,
		command
	};

	// functions
	void execute_relative(midi_message& in_msg);
	void execute_range(midi_message& in_msg);
	void execute_fixed(midi_message& in_msg);

	void modify_up(midi_message& in_msg, bool in_fast);
	void modify_down(midi_message& in_msg, bool in_fast);

	[[nodiscard]] float check_value_min_max(float in_value, float in_modifier) const;

	// members
	encoder_mode m_enc_mode {encoder_mode::relative};

	encoder_map_type m_enc_map_type {encoder_map_type::dataref};

	int m_delay_counter {0};
	int m_delay {-1};

	bool m_data_2_prev_set {false};
	unsigned char m_data_2_prev {};

	unsigned char m_data_2_up {};
	unsigned char m_data_2_down {};

	unsigned char m_data_2_min {MIDI_DATA_2_MIN};
	unsigned char m_data_2_max {MIDI_DATA_2_MAX};

	std::string m_dataref {};

	float m_modifier_up {0.0f};
	float m_modifier_down {0.0f};

	float m_modifier_fast_up {0.0f};
	float m_modifier_fast_down {0.0f};

	bool m_value_min_defined {false};
	bool m_value_max_defined {false};

	float m_value_min {0.0f};
	float m_value_max {0.0f};

	bool m_value_wrap {false};

	std::string m_command_up {};
	std::string m_command_down {};

	std::string m_command_fast_up {};
	std::string m_command_fast_down {};

	std::unique_ptr<label> m_label;
};

} // Namespace xmidictrl

#endif // XMC_MAP_IN_ENC_H
