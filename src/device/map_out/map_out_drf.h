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

#ifndef XMC_MAP_OUT_DRF_H
#define XMC_MAP_OUT_DRF_H

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

class map_out_drf : public map_out {
public:
    explicit map_out_drf(environment& in_env);
    ~map_out_drf() override;

    map_out_type type() override;

    void set_dataref(std::string_view in_dataref);
    void set_dataref(std::vector<std::string> in_dataref);

    void set_data_2_on(unsigned char in_data_2_on);
    void set_data_2_off(unsigned char in_data_2_off);

    void read_config(text_logger& in_log, toml::value& in_data) override;
    bool check(text_logger& in_log, const device_settings& in_dev_settings) override;

    std::unique_ptr<map_result> execute(map_param* in_param) override;

    std::string map_text_drf() override;
    std::string map_text_parameter() override;

protected:
    std::string build_mapping_text(bool in_short) override;

private:
    // constants
    static constexpr std::string_view c_cfg_data_2_off {"data_2_off"};
    static constexpr std::string_view c_cfg_data_2_on {"data_2_on"};

    static constexpr std::string_view c_cfg_send_on {"send_on"};
    static constexpr std::string_view c_cfg_send_off {"send_off"};

    // members
    std::vector<std::string> m_datarefs {};
    std::map<std::string, std::string> m_xp_values {};

    std::set<std::string> m_values_on {};
    std::set<std::string> m_values_off {};

    send_mode m_send_on {send_mode::one};
    send_mode m_send_off {send_mode::all};

    unsigned char m_data_2_on {MIDI_DATA_2_MAX};
    unsigned char m_data_2_off {MIDI_DATA_2_MIN};

    char m_previous_data_2 {MIDI_NONE};
};

} // Namespace xmidictrl

#endif // XMC_MAP_OUT_DRF_H