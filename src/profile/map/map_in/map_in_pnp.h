//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#ifndef XMC_MAP_IN_PNP_H
#define XMC_MAP_IN_PNP_H

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

class map_in_pnp : public map_in {
public:
    explicit map_in_pnp(environment &in_env);
    ~map_in_pnp() override = default;

    map_in_type type() override;

    void set_time_received();
    void set_time_released();

    void read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config) override;
    bool check(text_logger &in_log) override;

    bool execute(midi_message &in_msg, std::string_view in_sl_value) override;

    std::string map_text_cmd_drf() override;
    std::string map_text_parameter() override;

protected:
    std::string build_mapping_text(bool in_short) override;

private:
    enum class command_type {
        none,
        push,
        pull
    };

    void reset();

    command_type m_command_type {command_type::none};
    time_point m_time_command {time_point::min()};

    std::atomic<time_point> m_time_received {time_point::min()};
    std::atomic<time_point> m_time_released {time_point::min()};

    std::string m_dataref_push {};
    std::string m_dataref_pull {};

    std::vector<std::string> m_values_push {};
    std::vector<std::string> m_values_pull {};

    std::string m_command_push {};
    std::string m_command_pull {};
};

} // Namespace xmidictrl

#endif // XMC_MAP_IN_PNP_H
