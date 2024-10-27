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

#ifndef XMC_LABEL_H
#define XMC_LABEL_H

// Standard
#include <map>
#include <memory>
#include <string>

// XMidiCtrl
#include "environment.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class label {
public:
	// constants
	static constexpr std::string_view c_cfg_label {"label"};

	explicit label(environment& in_env);
	~label() = default;

	void read_config(text_logger& in_log,
					 toml::value& in_data,
					 toml::value& in_config,
					 std::string_view in_dataref = {},
					 std::string_view in_cfg_label = c_cfg_label);
	bool check(text_logger& in_log, std::string_view in_source_line);

	void display_label(text_logger& in_log);

	std::string id() const;

private:
	// constants
	static constexpr std::string_view c_cfg_dataref {"dataref"};
	static constexpr std::string_view c_cfg_text {"text"};
	static constexpr std::string_view c_cfg_value {"value"};
	static constexpr std::string_view c_cfg_values {"values"};

	// members
	environment& m_env;

	std::string m_id {};
	std::string m_dataref {};
	std::string m_text {};

	std::map<std::string, std::string> m_values {};
};

} // Namespace xmidictrl

#endif // XMC_LABEL_H
