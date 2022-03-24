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

#ifndef MAP_IN_DRF_H
#define MAP_IN_DRF_H

// Standard
#include <string>
#include <string_view>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"

namespace xmidictrl {

class map_in_drf : public map_in {
public:
    explicit map_in_drf(xplane *xp);
    ~map_in_drf() override = default;

    map_type type() override;

    void read_config(message_handler *messages, toml::value &data) override;
    bool check() override;

    bool execute(message_handler *messages, midi_message &msg, std::string_view sl_value) override;

private:
    dataref_mode m_mode {dataref_mode::toggle};

    std::string m_dataref {};

    std::vector<std::string> m_values {};
};

} // Namespace xmidictrl

#endif // MAP_IN_DRF_H