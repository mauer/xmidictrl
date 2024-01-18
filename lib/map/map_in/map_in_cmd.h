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

#ifndef XMC_MAP_IN_CMD_H
#define XMC_MAP_IN_CMD_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "environment.h"
#include "map.h"
#include "map_in_label.h"
#include "midi_message.h"
#include "text_logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in_cmd : public map_in_label {
public:
	explicit map_in_cmd(environment& in_env);
	~map_in_cmd() override = default;

	map_in_type type() override;

	[[nodiscard]] unsigned char data_2_on() const;
	[[nodiscard]] unsigned char data_2_off() const;

	void read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config) override;
	bool check(text_logger& in_log, const device_settings& in_dev_settings) override;

	std::unique_ptr<map_result> execute(map_param* in_param) override;

	std::string map_text_cmd_drf() override;
	std::string map_text_parameter() override;

protected:
	std::string build_mapping_text(bool in_short) override;

private:
	// constants
	static constexpr std::string_view c_cfg_command {"command"};

	static constexpr std::string_view c_cfg_data_2_on {"data_2_on"};
	static constexpr std::string_view c_cfg_data_2_off {"data_2_off"};

	// members
	std::string m_command {};

	unsigned char m_data_2_on {MIDI_DATA_2_MAX};
	unsigned char m_data_2_off {MIDI_DATA_2_MIN};
};

} // Namespace xmidictrl

#endif // XMC_MAP_IN_CMD_H
