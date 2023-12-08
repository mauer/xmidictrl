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

#ifndef XMC_MAP_OUT_CON_H
#define XMC_MAP_OUT_CON_H

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

class map_out_con : public map_out {
public:
    explicit map_out_con(environment& in_env);
    ~map_out_con() override = default;

    map_out_type type() override;

    void set_data_2(unsigned char in_data_2);

    void read_config(text_logger& in_log, toml::value& in_data) override;
    bool check(text_logger& in_log) override;

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
    static constexpr std::string_view c_cfg_data_2 {"data_2"};
    static constexpr std::string_view c_cfg_velocity {"velocity"};

    // members
    unsigned char m_data_2 {MIDI_DATA_2_MAX};
};

} // Namespace xmidictrl

#endif // XMC_MAP_OUT_CON_H