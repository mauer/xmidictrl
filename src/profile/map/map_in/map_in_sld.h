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

#ifndef XMC_MAP_IN_SLD_H
#define XMC_MAP_IN_SLD_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "environment.h"
#include "map_in.h"
#include "text_logger.h"
#include "midi_message.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in_sld : public map_in {
public:
    explicit map_in_sld(environment &in_env);
    ~map_in_sld() override = default;

    map_in_type type() override;
    std::string type_as_string() override;

    void read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config) override;
    bool check(text_logger &in_log) override;

    bool execute(midi_message &in_msg, std::string_view in_sl_value) override;

    std::string map_text_cmd_drf() override;
    std::string map_text_parameter() override;

protected:
    std::string build_mapping_text(bool in_short) override;

private:
    std::string m_dataref {};
    float m_value_min {0.0f};
    float m_value_max {1.0f};

    std::string m_command_prev {};

    std::string m_command_up {};
    std::string m_command_middle {};
    std::string m_command_down {};
};

} // Namespace xmidictrl

#endif // XMC_MAP_IN_SLD_H
