//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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
#include "map_in_enc.h"
#include "midi_message.h"
#include "test_helper.h"
#include "text_logger.h"

using namespace xmidictrl;

TEST_CASE("Test Inbound Mapping for encoders in command mode with type range")
{
	// create our test environment
	auto env = test_helper::create_environment();
/*
	// create test mapping
    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
        ch = 16
        cc = 20
		encoder_mode = "range"
        command_up = "sim/autopilot/heading_up"
        command_down = "sim/autopilot/heading_down"
    )"_toml;

    auto map = std::make_unique<map_in_enc>(*env);
    map->read_config(*env->log, cfg, cfg);

	// check the mapping
	auto* dev_settings = new device_settings();
	CHECK(map->check(*env->log, *dev_settings));

	// create midi message
	auto msg = std::make_shared<midi_message>(*env->log, midi_direction::in);

	SUBCASE("Test command up")
	{
		auto param = std::make_unique<map_param>(msg, *env->log);

		for (int i = 0; i < 200; i++) {
			unsigned char data_2 = static_cast<unsigned char>(i <= 127 ? i : 127);
			msg->set_data_2(data_2);

			// execute the mapping
			//auto result = map->execute(param.get());
			//CHECK(result->completed);

			//CHECK(env->env->cmd_tests().last_command() == "sim/autopilot/heading_up");
		}
	} */
}