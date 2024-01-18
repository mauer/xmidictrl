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
#include "midi_message.h"
#include "text_logger.h"

using namespace xmidictrl;

TEST_CASE("Test Inbound Mapping for commands with default data 2")
{
	auto log = std::make_unique<text_logger>();
	auto env = std::make_unique<env_tests>(*log);

    auto* dev_settings = new device_settings();

    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
        ch = 16
        cc = 20
        command = "sim/autopilot/enable"
    )"_toml;

    auto map = std::make_unique<map_in_cmd>(*env);
    map->read_config(*log, cfg, cfg);

    auto msg = std::make_shared<midi_message>(*log, midi_direction::in);
    msg->set_data_2(MIDI_DATA_2_MAX);

    CHECK(map->check(*log, *dev_settings));

	auto param = std::make_unique<map_param>(msg, *log);

    auto result = map->execute(param.get());
	CHECK(result->completed);

	CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

    msg->set_data_2(MIDI_DATA_2_MIN);

	result = map->execute(param.get());
	CHECK(result->completed);

    CHECK(env->cmd_tests().current_command() == "");
    CHECK(env->cmd_tests().last_command() == "sim/autopilot/enable");
}


TEST_CASE("Test Inbound Mapping for commands with sublayer")
{
	auto log = std::make_unique<text_logger>();
	auto env = std::make_unique<env_tests>(*log);

    auto* dev_settings = new device_settings();

    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
        ch = 16
        cc = 20
        sl = "1"
        command = "sim/autopilot/enable"
    )"_toml;

    auto map = std::make_unique<map_in_cmd>(*env);
    map->read_config(*log, cfg, cfg);

    auto msg = std::make_shared<midi_message>(*log, midi_direction::in);
    msg->set_data_2(MIDI_DATA_2_MAX);

	CHECK(map->check(*log, *dev_settings));

	auto param = std::make_unique<map_param>(msg, *log, "2");
	auto result = map->execute(param.get());

	CHECK(result->completed);
    CHECK(env->cmd_tests().current_command() == "");

    param->sl_value = "1";
	result = map->execute(param.get());

	CHECK(result->completed);
    CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

    msg->set_data_2(MIDI_DATA_2_MIN);

	param->sl_value = "2";
	result = map->execute(param.get());

	CHECK(result->completed);
    CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

	param->sl_value = "1";
	result = map->execute(param.get());

	CHECK(result->completed);
	CHECK(env->cmd_tests().current_command() == "");
    CHECK(env->cmd_tests().last_command() == "sim/autopilot/enable");
}


TEST_CASE("Test Inbound Mapping for commands with custom data 2")
{
    auto log = std::make_unique<text_logger>();
    auto env = std::make_unique<env_tests>(*log);

    auto dev_settings = std::make_unique<device_settings>();

    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
        ch = 16
        cc = 20
        command = "sim/autopilot/enable"
        data_2_on = 60
        data_2_off = 10
    )"_toml;

    auto map = std::make_unique<map_in_cmd>(*env);
    map->read_config(*log, cfg, cfg);

    auto msg = std::make_shared<midi_message>(*log, midi_direction::in);
    msg->set_data_2(60);

    CHECK(map->check(*log, *dev_settings));

	auto param = std::make_unique<map_param>(msg, *log);
	auto result = map->execute(param.get());

	CHECK(result->completed);
    CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

    msg->set_data_2(0);

	result = map->execute(param.get());

	CHECK(result->completed);
    CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

    msg->set_data_2(10);

	result = map->execute(param.get());

	CHECK(result->completed);
    CHECK(env->cmd_tests().current_command() == "");
    CHECK(env->cmd_tests().last_command() == "sim/autopilot/enable");
}