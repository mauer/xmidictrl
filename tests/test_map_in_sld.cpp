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

// doctest
#include "doctest/doctest.h"

// toml11
#include "toml.hpp"

// XMidiCtrl
#include "env_tests.h"
#include "map_in_sld.h"
#include "midi_message.h"
#include "text_logger.h"

using namespace xmidictrl;

TEST_CASE("Test Inbound Mapping for sliders with commands")
{
    auto* log = new text_logger();
    auto* env = new env_tests(*log);

    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
        ch = 16
        cc = 20
        command_up = "sim/flaps/up"
        command_middle = "sim/flaps/neutral"
        command_down = "sim/flaps/down"
    )"_toml;

    auto map = new map_in_sld(*env);
    map->read_config(*log, cfg, cfg);

    auto msg = new midi_message(*log, midi_direction::in);
    msg->set_data_2(MIDI_DATA_2_MAX);

    CHECK(map->check(*log));
    CHECK(map->execute(*msg, ""));
    CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

    msg->set_data_2(MIDI_DATA_2_MIN);
    CHECK(map->execute(*msg, ""));
    CHECK(env->cmd_tests().current_command() == "");
    CHECK(env->cmd_tests().last_command() == "sim/autopilot/enable");
}
