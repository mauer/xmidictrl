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

#include "virtual_device.h"

// XMidiCtrl
#include "device_list.h"
#include "map_in_cmd.h"
#include "map_in_pnp.h"
#include "plugin.h"
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
virtual_device::virtual_device(text_logger& in_text_log,
                               midi_logger& in_midi_log,
                               const std::shared_ptr<device_settings>& in_settings)
    : device(in_text_log, in_midi_log, in_settings)
{
    in_text_log.info("Created new virtual MIDI device :: Name = '" + std::string(in_settings->name) + "'");
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the device type
 */
device_type virtual_device::type()
{
    return device_type::virtual_device;
}


/**
 * Process an inbound midi message
 */
void virtual_device::process_inbound_message(unsigned char in_channel, unsigned char in_data, unsigned char in_velocity)
{
    // create a regular midi message
    std::shared_ptr<midi_message> midi_msg = std::make_shared<midi_message>(text_log(), midi_direction::in);
    midi_msg->create_cc_message(in_channel, in_data, in_velocity);

    // log MIDI in message
    midi_log().add(midi_msg);
    text_log().debug("Inbound message from virtual device '" + settings().name + "' :: "
                     + "Status = '" + std::to_string(midi_msg->status()) + "', "
                     + "Data 1 = '" + std::to_string(midi_msg->data_1()) + "', "
                     + "Data 2 = '" + std::to_string(midi_msg->data_2()) + "'");

    if (!midi_msg->check())
        return;

    // check for mappings
    auto mappings = mapping_in().find(midi_msg->key());

    for (auto& mapping: mappings) {
        bool add_task = false;

        midi_msg->add_mapping(mapping);

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
            task->map = mapping;

            plugin::instance().add_inbound_task(task);
        }
    }
}

} // Namespace xmidictrl
