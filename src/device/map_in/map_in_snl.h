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

#ifndef XMC_MAP_IN_SNL_H
#define XMC_MAP_IN_SNL_H

// Standard
#include <atomic>
#include <memory>
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "environment.h"
#include "map_in.h"
#include "midi_message.h"
#include "text_logger.h"
#include "types.h"

#ifdef min
    #undef min // prevent clash with time_point::min() later
#endif

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in_snl : public map_in {
public:
    explicit map_in_snl(environment& in_env, bool in_legacy_mode);
    ~map_in_snl() override = default;

    map_in_type type() override;

    void set_time_received();
    void set_time_released();

    void read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config) override;
    bool check(text_logger& in_log, const midi_device_settings& in_dev_settings) override;

	std::unique_ptr<map_result> execute(map_param* in_param) override;

    std::string map_text_label() override;
    std::string map_text_cmd_drf() override;
    std::string map_text_parameter() override;

protected:
    std::string build_mapping_text(bool in_short) override;

private:
    // constants
    static constexpr std::string_view c_cfg_command_push {"command_push"};
    static constexpr std::string_view c_cfg_command_pull {"command_pull"};

	static constexpr std::string_view c_cfg_dataref_push {"dataref_push"};
	static constexpr std::string_view c_cfg_dataref_pull {"dataref_pull"};

	static constexpr std::string_view c_cfg_values_push {"values_push"};
	static constexpr std::string_view c_cfg_values_pull {"values_pull"};

	static constexpr std::string_view c_cfg_command_short {"command_short"};
	static constexpr std::string_view c_cfg_command_long {"command_long"};

	static constexpr std::string_view c_cfg_dataref_short {"dataref_short"};
	static constexpr std::string_view c_cfg_dataref_long {"dataref_long"};

	static constexpr std::string_view c_cfg_values_short {"values_short"};
	static constexpr std::string_view c_cfg_values_long {"values_long"};

	static constexpr std::string_view c_cfg_label_short {"label_short"};
	static constexpr std::string_view c_cfg_label_long {"label_long"};

    // enumerations
    enum class command_type {
        none,
        cmd_short,
        cmd_long
    };

    void reset();

	bool m_legacy_mode;

    command_type m_command_type {command_type::none};
    time_point m_time_command {time_point::min()};

    std::atomic<time_point> m_time_received {time_point::min()};
    std::atomic<time_point> m_time_released {time_point::min()};

    std::string m_dataref_short {};
    std::string m_dataref_long {};

    std::vector<std::string> m_values_short {};
    std::vector<std::string> m_values_long {};

    std::string m_command_short {};
    std::string m_command_long {};

	std::unique_ptr<label> m_label_short;
	std::unique_ptr<label> m_label_long;
};

} // Namespace xmidictrl

#endif // XMC_MAP_IN_SNL_H
