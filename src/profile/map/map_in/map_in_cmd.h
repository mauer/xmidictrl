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
#include "text_logger.h"
#include "midi_message.h"
#include "xplane.h"

namespace xmidictrl {

class map_in_cmd : public map_in {
public:
    explicit map_in_cmd(xplane &in_xp);
    ~map_in_cmd() override = default;

    map_type type() override;

    [[nodiscard]] unsigned int velocity_on() const;
    [[nodiscard]] unsigned int velocity_off() const;

    void read_config(text_logger &in_log, toml::value &in_data, device &in_device, toml::value &in_config) override;
    bool check(text_logger &in_log) override;

    bool execute(midi_message &in_msg, std::string_view in_sl_value) override;

protected:
    std::string build_mapping_text() override;

private:
    std::string m_command {};

    unsigned int m_velocity_on {MIDI_VELOCITY_MAX};
    unsigned int m_velocity_off {MIDI_VELOCITY_MIN};
};

} // Namespace xmidictrl

#endif // MAP_IN_CMD_H
