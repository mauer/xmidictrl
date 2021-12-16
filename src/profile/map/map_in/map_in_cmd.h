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

#ifndef MAP_IN_CMD_H
#define MAP_IN_CMD_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map.h"
#include "map_in.h"
#include "midi_message.h"
#include "xplane.h"

namespace xmidictrl {

class map_in_cmd : public map_in {
public:
    explicit map_in_cmd(std::shared_ptr<xplane> xp);
    ~map_in_cmd() override = default;

    map_type type() override;

    void set_command(std::string_view command);
    [[nodiscard]] std::string_view command() const;

    void read_config(toml::value &settings) override;
    bool check() override;

    void execute(midi_message &msg) override;

private:
    std::string m_command {};
};

} // Namespace xmidictrl

#endif // MAP_IN_CMD_H
