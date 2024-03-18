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

#include "virtual_midi_device.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "inbound_task.h"
#include "map_in_cmd.h"
#include "map_in_pnp.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
virtual_midi_device::virtual_midi_device(text_logger& in_text_log,
										 midi_logger& in_midi_log,
										 environment& in_env,
										 std::unique_ptr<device_settings> in_settings)
	: device(in_text_log, in_env, std::move(in_settings))
	, m_midi_log(in_midi_log)
{
	in_text_log.info(fmt::format("Created new virtual MIDI device :: Name = '{}'", settings().name));
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the device type
 */
device_type virtual_midi_device::type()
{
	return device_type::virtual_midi_device;
}


/**
 * Process an inbound midi message
 */
void virtual_midi_device::process_inbound_message(unsigned char in_channel,
												  unsigned char in_data,
												  unsigned char in_velocity)
{
	// create a regular midi message
	std::shared_ptr<midi_message> midi_msg = std::make_shared<midi_message>(text_log(), midi_direction::in);
	midi_msg->create_cc_message(in_channel, in_data, in_velocity);

	// log MIDI in message
	m_midi_log.add(midi_msg);
	text_log().debug(fmt::format("Inbound message from virtual device '{}' :: "
								 "Status = '{}', Data 1 = '{}', Data 2 = '{}'",
								 settings().name,
								 midi_msg->status(),
								 midi_msg->data_1(),
								 midi_msg->data_2()));

	if (!midi_msg->check())
		return;

	// check for mappings
	auto mappings =
		mapping_in().find(map::build_map_key(midi_msg->channel(), midi_msg->type_as_code(), midi_msg->data_1()));

	for (auto& mapping: mappings) {
		bool add_task = false;

		midi_msg->add_mapping_text(mapping->map_text());

		// for push and pull we have to wait until the command has ended
		if (mapping->type() == map_in_type::push_pull) {
			switch (midi_msg->data_2()) {
				case MIDI_DATA_2_MAX: {
					std::shared_ptr<map_in_pnp> pnp = std::static_pointer_cast<map_in_pnp>(mapping);
					pnp->set_time_received();
					add_task = true;
					break;
				}

				case MIDI_DATA_2_MIN: {
					// no additional task is required, it's already in the event queue waiting for the
					// release time point
					std::shared_ptr<map_in_pnp> pnp = std::static_pointer_cast<map_in_pnp>(mapping);
					pnp->set_time_released();
					break;
				}

				default:
					text_log().warn("Invalid MIDI velocity '" + std::to_string(midi_msg->data_2())
									+ "' for a Push&Pull mapping");
					break;
			}

		} else if (mapping->type() == map_in_type::command) {
			std::shared_ptr<map_in_cmd> cmd = std::static_pointer_cast<map_in_cmd>(mapping);
			add_task = true;
		} else {
			add_task = true;
		}

		// push to event handler
		if (add_task) {
			std::shared_ptr<inbound_task> task = std::make_shared<inbound_task>();
			task->msg = midi_msg;
			task->mapping = mapping;

			env().worker().add_task(task);
		}
	}
}

} // Namespace xmidictrl
