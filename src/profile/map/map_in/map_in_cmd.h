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
#include "message_handler.h"
#include "midi_message.h"
#include "xplane.h"

namespace xmidictrl {

class map_in_cmd : public map_in {
public:
    explicit map_in_cmd(xplane *xp);
    ~map_in_cmd() override = default;

    map_type type() override;

    void set_command(std::string_view command);
    [[nodiscard]] std::string_view command() const;

    void set_velocity_on(int velocity_on);
    [[nodiscard]] unsigned int velocity_on() const;

    void set_velocity_off(int value_off);
    [[nodiscard]] unsigned int velocity_off() const;

    void read_config(message_handler *messages, toml::value &data) override;
    bool check() override;

    bool execute(message_handler *messages, midi_message &msg, std::string_view sl_value) override;

private:
    std::string m_command {};

    unsigned int m_velocity_on {127};
    unsigned int m_velocity_off {0};
};

} // Namespace xmidictrl

#endif // MAP_IN_CMD_H
