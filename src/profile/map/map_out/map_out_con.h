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

#ifndef MAP_OUT_CON_H
#define MAP_OUT_CON_H

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

class map_out_con : public map_out {
public:
    explicit map_out_con(environment &in_env);
    ~map_out_con() override;

    map_type type() override;

    void set_velocity(int in_velocity);

    void read_config(text_logger &in_log, toml::value &in_data) override;
    bool check(text_logger &in_log) override;

    std::shared_ptr<outbound_task> execute(text_logger &in_log, mode_out in_mode) override;
    std::shared_ptr<outbound_task> reset() override;

protected:
    std::string build_mapping_text() override;

private:
    unsigned int m_velocity {127};
};

} // Namespace xmidictrl

#endif // MAP_OUT_CON_H