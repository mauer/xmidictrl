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

#include "midi_device.h"

#include <utility>

// XMidiCtrl
#include "device_list.h"
#include "map_in_cmd.h"
#include "map_in_pnp.h"
#include "plugin.h"
#include "conversions.h"

// Make sure min is not defined as a macro otherwise time_point::min() will not compile
#if defined(_MSC_VER) && defined(min)
    #undef min
#endif

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
midi_device::midi_device(text_logger& in_text_log,
                         midi_logger& in_midi_log,
                         std::shared_ptr<device_settings> in_settings)
    : device(in_text_log, in_midi_log, std::move(in_settings))
{
    try {
        // create midi classes
        m_midi_in = std::make_unique<RtMidiIn>();
        m_midi_out = std::make_unique<RtMidiOut>();

        text_log().info("Created new MIDI device :: Name = '" + settings().name + "', "
                        + "Port In = '" + std::to_string(settings().port_in) + "', "
                        + "Port Out = '" + std::to_string(settings().port_out) + "'");
    } catch (RtMidiError& error) {
        // we should never reach this, but let's be on the safeW side
        text_log().error("Error creating MIDI connections for device '" + settings().name + "'");
        text_log().error(error.what());
    }
}


/**
 * Destructor
 */
midi_device::~midi_device()
{
    close_connections();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the device type
 */
device_type midi_device::type()
{
    return device_type::midi_device;
}


/**
 * Add a new init mapping to the device
 */
void midi_device::add_init_map(std::shared_ptr<map_init>& in_mapping)
{
    m_map_init.add(in_mapping);
}


/**
 * Add a new outbound mapping to the device
 */
void midi_device::add_outbound_map(std::shared_ptr<map_out>& in_mapping)
{
    m_map_out.add(in_mapping);
}


/**
 * Open connections for midi in and out
 */
bool midi_device::open_connections()
{
    // open midi in
    if (mapping_in().size() > 0) {
        try {
            m_midi_in->openPort(settings().port_in);
            m_midi_in->ignoreTypes(false, false, false);
            m_midi_in->setCallback(&midi_device::midi_callback, this);

            text_log().info("Inbound port '" + std::to_string(settings().port_in)
                            + "' opened for device '" + settings().name + "'");

        } catch (const RtMidiError& error) {
            text_log().error("Could not open inbound port '" + std::to_string(settings().port_in)
                             + "' for device '" + settings().name + "'");
            text_log().error(error.what());

            return false;
        }
    } else {
        text_log().debug("No inbound mappings found, skip opening inbound port for device '" + settings().name + "'");
    }

    // open midi out
    if (m_map_out.size() > 0) {
        try {
            m_midi_out->openPort(settings().port_out);

            text_log().info("Outbound port '" + std::to_string(settings().port_out) + "' opened for device '"
                            + settings().name + "'");

        } catch (const RtMidiError& error) {
            text_log().error("Could not open outbound port '" + std::to_string(settings().port_out)
                             + "' for device '" + settings().name + "'");
            text_log().error(error.what());

            return false;
        }

        create_outbound_thread();
    } else {
        text_log().debug("No outbound mappings found, skip opening outbound port for device '"
                         + settings().name + "'");
    }

    // connections successfully established
    return true;
}


/**
 * Close midi connections
 */
void midi_device::close_connections()
{
    if (m_midi_in != nullptr) {
        if (m_midi_in->isPortOpen()) {
            m_midi_in->closePort();

            text_log().info("Closed inbound port '" + std::to_string(settings().port_in) + "' for device '"
                            + settings().name + "'");
        }
    }

    if (m_outbound_thread != nullptr) {
        m_exit_outbound_thread.store(true);
        m_new_outbound_task.notify_one();

        if (m_outbound_thread->joinable())
            m_outbound_thread->join();
    }

    if (m_midi_out != nullptr) {
        if (m_midi_out->isPortOpen()) {
            m_midi_out->closePort();

            text_log().info("Closed outbound port '" + std::to_string(settings().port_out)
                            + "' for device '" + settings().name + "'");
        }
    }
}


/**
 * Callback method for midi inbound events
 */
void midi_device::midi_callback([[maybe_unused]] double in_deltatime,
                                std::vector<unsigned char>* in_message,
                                void* in_userdata)
{
    if (in_userdata != nullptr) {
        auto* dev = static_cast<midi_device*>(in_userdata);
        dev->process_inbound_message(in_message);
    }
}


/**
 * Process all init midi mappings
 */
void midi_device::process_init_mappings()
{
    for (auto& mapping: m_map_init) {
        std::shared_ptr<outbound_task> task = mapping->execute();

        if (task == nullptr)
            continue;

        if (m_midi_out != nullptr && m_midi_out->isPortOpen())
            add_outbound_task(task);
    }
}


/**
 * Process an inbound midi message
 */
void midi_device::process_inbound_message(std::vector<unsigned char>* in_message)
{
    // read midi message
    if (in_message->size() > 1) {
        std::shared_ptr<midi_message> midi_msg = std::make_shared<midi_message>(text_log(), midi_direction::in);
        midi_msg->parse_message(in_message);

        midi_msg->set_time(std::chrono::system_clock::now());
        midi_msg->set_port(settings().port_in);

        // log MIDI in message
        midi_log().add(midi_msg);
        text_log().debug("Inbound message from device '" + settings().name + "' on port '"
                         + std::to_string(settings().port_in) + "' :: "
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
            if (mapping->type() == map_type::push_pull) {
                switch (midi_msg->data_2()) {
                    case MIDI_VELOCITY_MAX: {
                        std::shared_ptr<map_in_pnp> pnp = std::static_pointer_cast<map_in_pnp>(mapping);
                        pnp->set_time_received();
                        add_task = true;
                        break;
                    }

                    case MIDI_VELOCITY_MIN: {
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

            } else if (mapping->type() == map_type::command) {
                std::shared_ptr<map_in_cmd> cmd = std::static_pointer_cast<map_in_cmd>(mapping);

                // lock the current channel and midi type for outgoing messages
                if (midi_msg->data_2() == cmd->velocity_on())
                    m_outbound_locked.insert(midi_msg->key());
                else if (midi_msg->data_2() == cmd->velocity_off())
                    m_outbound_locked.erase(midi_msg->key());

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
}


/**
 * Process all outbound midi mappings
 */
void midi_device::process_outbound_mappings(text_logger& in_log)
{
    if (settings().mode_out == mode_out::permanent) {
        if (m_time_sent != time_point::min()) {
            std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - m_time_sent;

            // wait half a second before sending another message
            if (elapsed_seconds.count() < settings().outbound_delay)
                return;
        }
    }

    for (auto& mapping: m_map_out) {
        if (m_outbound_locked.contains(mapping->get_key()))
            continue;

        std::shared_ptr<outbound_task> task = mapping->execute(in_log, settings().mode_out);

        if (task == nullptr)
            continue;

        if (m_midi_out != nullptr && m_midi_out->isPortOpen())
            add_outbound_task(task);
    }

    if (settings().mode_out == mode_out::permanent)
        m_time_sent = std::chrono::system_clock::now();
}


/**
 * Process a reset off all outbound mappings
 */
void midi_device::process_outbound_reset()
{
    for (auto& mapping: m_map_out) {
        std::shared_ptr<outbound_task> task = mapping->reset();

        if (task == nullptr)
            continue;

        if (m_midi_out != nullptr && m_midi_out->isPortOpen())
            add_outbound_task(task);
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create a thread to send outbound messages
 */
void midi_device::create_outbound_thread()
{
    if (m_outbound_thread == nullptr)
        m_outbound_thread = std::make_unique<std::thread>(&midi_device::process_outbound_tasks, this);
}


/**
 * Process all outbound tasks in the queue
 */
void midi_device::process_outbound_tasks()
{
    while (!m_exit_outbound_thread.load()) {
        // wait for a task to be added to the queue
        std::shared_ptr<midi_message> msg;
        {
            std::unique_lock<std::mutex> lock(m_outbound_mutex);

            while (m_outbound_msg.empty()) {
                m_new_outbound_task.wait_for(lock, std::chrono::seconds(10));

                if (m_exit_outbound_thread.load()) {
                    text_log().debug("Exit outbound thread is enabled (wait loop)");
                    break;
                }
            }

            if (m_exit_outbound_thread.load()) {
                text_log().debug("Exit outbound thread is enabled (task loop)");
                continue;
            }

            if (m_outbound_msg.empty())
                continue;

            msg = m_outbound_msg.front();
            m_outbound_msg.pop();
        }

        if (msg != nullptr) {
            std::vector<unsigned char> midi_out;
            midi_out.push_back(msg->status());
            midi_out.push_back(msg->data_1());
            midi_out.push_back(msg->data_2());

            try {
                m_midi_out->sendMessage(&midi_out);
            } catch (const RtMidiError& error) {
                text_log().error("MIDI device '" + settings().name + "' :: " + error.what());
                text_log().error("Lost connection to MIDI device, stopped sending outbound messages");

                m_midi_out->closePort();
                m_exit_outbound_thread.store(true);
            }
        }
    }

    text_log().debug("Exit outbound thread");
}


/**
 * Add an outbound task to the queue
 */
void midi_device::add_outbound_task(const std::shared_ptr<outbound_task>& in_task)
{
    if (m_exit_outbound_thread.load()) {
        text_log().debug("Outbound message rejected, as outbound thread is going to terminate");
        return;
    }

    std::shared_ptr<midi_message> msg = std::make_shared<midi_message>(text_log(), midi_direction::out);

    msg->set_time(std::chrono::system_clock::now());
    msg->set_port(settings().port_out);

    switch (in_task->type) {
        case midi_msg_type::control_change:
            msg->set_status((int) (0xb0 | (in_task->channel - 1)));
            break;

        case midi_msg_type::note_on:
            msg->set_status((int) (0x90 | (in_task->channel - 1)));
            break;

        case midi_msg_type::note_off:
            // some MIDI device expect note_on for note_off with a velocity of 0. Lets check the settings
            if (settings().mode_note == mode_note::on)
                msg->set_status((int) (0x90 | (in_task->channel - 1)));
            else
                msg->set_status((int) (0x80 | (in_task->channel - 1)));
            break;

        case midi_msg_type::pitch_bend:
            msg->set_status((int) (0xe0 | (in_task->channel - 1)));
            break;

        case midi_msg_type::program_change:
            msg->set_status((int) (0xc0 | (in_task->channel - 1)));
            break;

        default:
            break;

    }

    msg->set_data_1(in_task->data);

    // some MIDI device expect note_on for note_off with a velocity of 0. Lets check the settings
    if (in_task->type == midi_msg_type::note_off && settings().mode_note == mode_note::on)
        msg->set_data_2(MIDI_VELOCITY_MIN);
    else
        msg->set_data_2(in_task->velocity);

    msg->add_mapping(in_task->mapping);

    // add message for internal list, but only if the dataref value has been changed, otherwise the logging will
    // increase very quickly
    if (in_task->data_changed) {
        midi_log().add(msg);
        text_log().debug("Outbound message for device '" + settings().name
                         + "' on port '" + std::to_string(settings().port_out) + "' :: "
                         + "Status = '" + std::to_string(msg->status()) + "', "
                         + "Data 1 = '" + std::to_string(msg->data_1()) + "', "
                         + "Data 2 = '" + std::to_string(msg->data_2()) + "'");
    }

    std::lock_guard<std::mutex> lock(m_outbound_mutex);

    m_outbound_msg.push(msg);
    m_new_outbound_task.notify_one();
}

} // Namespace xmidictrl
