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

#ifndef MAP_IN_SLD_H
#define MAP_IN_SLD_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"

namespace xmidictrl {

class map_in_sld : public map_in {
public:
    explicit map_in_sld(std::shared_ptr<xplane> xp);
    ~map_in_sld() override = default;

    map_type type() override;

    void set_command_up(std::string_view command);
    [[nodiscard]] std::string_view command_up() const;

    void set_command_middle(std::string_view command);
    [[nodiscard]] std::string_view command_middle() const;

    void set_command_down(std::string_view command);
    [[nodiscard]] std::string_view command_down() const;

    void read_config(toml::value &settings) override;
    bool check() override;

    bool execute(midi_message &msg, std::string_view sl_value) override;

private:
    std::string m_command_up {};
    std::string m_command_middle {};
    std::string m_command_down {};
};

} // Namespace xmidictrl

#endif // MAP_IN_SLD_H
