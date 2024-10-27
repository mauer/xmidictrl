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
#include "map_in_cbv.h"
#include "midi_message.h"
#include "text_logger.h"
#include "toml_utils.h"

using namespace xmidictrl;

TEST_CASE("Test Inbound Mapping for sliders with commands")
{
    auto log = std::make_unique<text_logger>();
    auto env = std::make_unique<env_tests>(*log);

    using namespace toml::literals::toml_literals;
    toml::value cfg = u8R"(
		[[device]]
		mapping_in = [
		  { ch = 11, cc = 1, type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" },
		  { ch = 11, cc = 2, type = "cbv", dataref = "sim/autopilot/mode", values = [{value = "0", command = "sim/autopilot/on"}, {value = "1", command = "sim/autopilot/off"}] }
		]
    )"_toml;

	auto profile_dev = toml::find<std::vector<toml::table>>(cfg, "device");

	for (size_t dev_no = 0; dev_no < profile_dev.size(); dev_no++) {
		toml::array mappings = profile_dev[dev_no]["mapping_in"].as_array();

		for (int map_no = 0; map_no < static_cast<int>(mappings.size()); map_no++) {
			// parse each mapping entry
			if (toml_utils::read_string(*log, mappings[map_no], "type") == "cbv") {
				auto map = new map_in_cbv(*env);
				auto dev_settings = std::make_unique<midi_device_settings>();

				map->read_config(*log, mappings[map_no], mappings[map_no]);
				CHECK(map->check(*log, *dev_settings));
			}
		}

	}


    //auto msg = new midi_message(*log, midi_direction::in);
    //msg->set_data_2(MIDI_DATA_2_MAX);


    //CHECK(map->execute(*msg, ""));
    //CHECK(env->cmd_tests().current_command() == "sim/autopilot/enable");

    //msg->set_data_2(MIDI_DATA_2_MIN);
    //CHECK(map->execute(*msg, ""));
    //CHECK(env->cmd_tests().current_command() == "");
    //CHECK(env->cmd_tests().last_command() == "sim/autopilot/enable");
}
