//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
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

#ifndef MAP_INIT_H
#define MAP_INIT_H

// Standard
#include <memory>

// XMidiCtrl
#include "environment.h"
#include "map.h"
#include "outbound_task.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_init : public map {
public:
    explicit map_init();
    ~map_init() override = default;

    void set_data_2(unsigned char in_data_2);
    unsigned char data_2() const;

    void read_config(text_logger& in_log, toml::value& in_data);
    bool check(text_logger& in_log) override;

    std::shared_ptr<outbound_task> execute();

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

#endif // MAP_INIT_H
