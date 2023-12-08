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
#include "map_in_cmd.h"
#include "text_logger.h"

using namespace xmidictrl;

TEST_CASE("Test Inbound Mapping for commands")
{
    text_logger* log = new text_logger();
    env_tests* env = new env_tests(*log);

    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
        ch = 16
        cc = 20
        command = "sim/autopilot/enable"
    )"_toml;

    auto map = new map_in_cmd(*env);
    map->read_config(*log, cfg, cfg);

    CHECK(map->check(*log));
}