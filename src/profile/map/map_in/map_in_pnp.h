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

#ifndef MAP_IN_PNP_H
#define MAP_IN_PNP_H

// Standard
#include <atomic>
#include <memory>
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"
#include "types.h"

namespace xmidictrl {

enum class command_type {
    none,
    push,
    pull
};

class map_in_pnp : public map_in {
public:
    explicit map_in_pnp(xplane *xp);
    ~map_in_pnp() override = default;

    map_type type() override;

    void set_command_push(std::string_view command);
    [[nodiscard]] std::string_view command_push() const;

    void set_command_pull(std::string_view command);
    [[nodiscard]] std::string_view command_pull() const;

    void set_time_received();
    void set_time_released();

    void read_config(message_handler *messages, toml::value &data) override;
    bool check() override;

    bool execute(message_handler *messages, midi_message &msg, std::string_view sl_value) override;

private:
    void reset();

    command_type m_command_type {command_type::none};
    time_point m_time_command {time_point::min()};

    std::atomic<time_point> m_time_received {time_point::min()};
    std::atomic<time_point> m_time_released {time_point::min()};

    std::string m_command_push {};
    std::string m_command_pull {};
};

} // Namespace xmidictrl

#endif // MAP_IN_PNP_H
