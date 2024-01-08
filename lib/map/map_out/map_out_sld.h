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

#ifndef XMC_MAP_OUT_SLD_H
#define XMC_MAP_OUT_SLD_H

// Standard
#include <set>
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "environment.h"
#include "map_out.h"
#include "outbound_task.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_out_sld : public map_out {
public:
    explicit map_out_sld(environment& in_env);
    ~map_out_sld() override;

    map_out_type type() override;

    void set_dataref(std::string_view in_dataref);

    void set_value_min(float in_value_min);
    void set_value_max(float in_value_max);

    void set_data_2_min(int in_data_2_min);
    void set_data_2_max(int in_data_2_max);

    void read_config(text_logger& in_log, toml::value& in_data) override;
    bool check(text_logger& in_log, const device_settings& in_dev_settings) override;

    std::shared_ptr<outbound_task> execute(text_logger& in_log,
                                           outbound_send_mode in_send_mode,
                                           std::string_view in_sl_value) override;
    std::shared_ptr<outbound_task> reset() override;

    std::string map_text_drf() override;
    std::string map_text_parameter() override;

protected:
    std::string build_mapping_text(bool in_short) override;

private:
    // constants
    static constexpr std::string_view c_cfg_data_2_max {"data_2_max"};
    static constexpr std::string_view c_cfg_data_2_min {"data_2_min"};

    // members
    std::string m_dataref {};

    bool m_first_execution {true};

    float m_xp_value {0.0f};

    float m_value_min {0.0f};
    float m_value_max {1.0f};

    unsigned int m_data_2_min {MIDI_DATA_2_MIN};
    unsigned int m_data_2_max {MIDI_DATA_2_MAX};
};

} // Namespace xmidictrl

#endif // XMC_MAP_OUT_SLD_H