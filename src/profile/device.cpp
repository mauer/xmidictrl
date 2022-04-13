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

#include "device.h"

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
device::device(text_logger &in_text_log,
               midi_logger &in_midi_log,
               std::string_view in_name,
               unsigned int in_port_in,
               unsigned int in_port_out,
               mode_out in_mode_out)
    : m_text_log(in_text_log),
      m_midi_log(in_midi_log),
      m_name(in_name),
      m_port_in(in_port_in),
      m_port_out(in_port_out),
      m_mode_out(in_mode_out)
{
    try {
        // create midi classes
        m_midi_in = std::make_unique<RtMidiIn>();
        m_midi_out = std::make_unique<RtMidiOut>();

        m_text_log.info("Created new MIDI device :: Name = '%s', Port In = '%i', Port Out = '%i'",
                        m_name.data(), in_port_in, in_port_out);

    } catch (RtMidiError &error) {
        // we should never reach this, but let's be on the safe side
        m_text_log.error("Error creating MIDI connections for device '%s'", m_name.c_str());
        m_text_log.error(error.what());
    }
}


/**
 * Destructor
 */
device::~device()
{
    close_connections();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a new inbound mapping to the device
 */
void device::add_inbound_map(std::shared_ptr<map_in> &in_mapping)
{
    m_map_in.add(in_mapping);
}


/**
 * Add a new outbound mapping to the device
 */
void device::add_outbound_map(std::shared_ptr<map_out> &in_mapping)
{
    m_map_out.add(in_mapping);
}


/**
 * Open connections for midi in and out
 */
bool device::open_connections()
{
    // open midi in
    if (m_map_in.size() > 0) {
        try {
            m_midi_in->openPort(m_port_in);
            m_midi_in->ignoreTypes(false, false, false);
            m_midi_in->setCallback(&device::midi_callback, this);

            m_text_log.info("Inbound port '%i' opened for device '%s'", m_port_in, m_name.c_str());

        } catch (const RtMidiError &error) {
            m_text_log.error("Could not open inbound port '%i' for device '%s'", m_port_in, m_name.c_str());
            m_text_log.error(error.what());

            return false;
        }
    }

    // open midi out
    if (m_map_out.size() > 0) {
        try {
            m_midi_out->openPort(m_port_out);

            m_text_log.info("Outbound port '%i' opened for device '%s'", m_port_out, m_name.c_str());

        } catch (const RtMidiError &error) {
            m_text_log.error("Could not open outbound port '%i' for device '%s'", m_port_in, m_name.c_str());
            m_text_log.error(error.what());

            return false;
        }

        create_outbound_thread();
    }

    // connections successfully established
    return true;
}


/**
 * Close midi connections
 */
void device::close_connections()
{
    if (m_midi_in != nullptr) {
        if (m_midi_in->isPortOpen()) {
            m_midi_in->closePort();

            m_text_log.info("Closed inbound port '%i' for device '%s'", m_port_in, m_name.c_str());
        }
    }

    if (m_outbound_thread != nullptr) {
        m_text_log.debug("Begin Terminate outbound thread '%i' for device '%s'",
                         m_outbound_thread->get_id(),
                         m_name.c_str());

        m_exit_outbound_thread.store(true);
        m_new_outbound_task.notify_one();

        if (m_outbound_thread->joinable())
            m_outbound_thread->join();

        m_text_log.debug("End Terminate outbound thread '%i' for device '%s'",
                         m_outbound_thread->get_id(),
                         m_name.c_str());
    }

    if (m_midi_out != nullptr) {
        if (m_midi_out->isPortOpen()) {
            m_midi_out->closePort();

            m_text_log.info("Closed outbound port '%i' for device '%s'", m_port_out, m_name.c_str());
        }
    }
}


/**
 * Callback method for midi inbound events
 */
void device::midi_callback([[maybe_unused]] double in_deltatime,
                           std::vector<unsigned char> *in_message,
                           void *in_userdata)
{
    if (in_userdata != nullptr) {
        auto *dev = static_cast<device *>(in_userdata);
        dev->process_inbound_message(in_message);
    }
}


/**
 * Process an inbound midi message
 */
void device::process_inbound_message(std::vector<unsigned char> *in_message)
{
    // read midi message
    if (in_message->size() > 1) {
        std::shared_ptr<midi_message> midi_msg = std::make_shared<midi_message>(m_text_log, midi_direction::in);
        midi_msg->parse_message(in_message);

        midi_msg->set_time(std::chrono::system_clock::now());
        midi_msg->set_port(m_port_in);

        // log MIDI in message
        m_midi_log.add(midi_msg);
        m_text_log.debug("Inbound message from device '%s' on port '%i' :: Status = '%i', Data 1 = '%i', Data 2 = '%i'",
                         m_name.c_str(), m_port_in, midi_msg->status(), midi_msg->data_1(), midi_msg->data_2());

        if (!midi_msg->check())
            return;

        // check for mappings
        auto mappings = m_map_in.find(midi_msg->key());

        for (auto &mapping: mappings) {
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
                        m_text_log.warn("Invalid MIDI velocity '%i' for a Push&Pull mapping", midi_msg->data_2());
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
void device::process_outbound_mappings(text_logger &in_log)
{
    if (m_mode_out == mode_out::permanent) {
        if (m_time_sent != time_point::min()) {
            std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - m_time_sent;

            if (elapsed_seconds.count() < 0.5f)
                return;
        }
    }

    for (auto &mapping: m_map_out) {
        if (m_outbound_locked.contains(mapping->get_key()))
            continue;

        std::shared_ptr<outbound_task> task = mapping->execute(in_log, m_mode_out);

        if (task == nullptr)
            continue;

        if (m_midi_out != nullptr && m_midi_out->isPortOpen())
            add_outbound_task(task);
    }

    if (m_mode_out == mode_out::permanent)
        m_time_sent = std::chrono::system_clock::now();
}


/**
 * Process a reset off all outbound mappings
 */
void device::process_outbound_reset()
{
    for (auto &mapping: m_map_out) {
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
void device::create_outbound_thread()
{
    if (m_outbound_thread == nullptr)
        m_outbound_thread = std::make_unique<std::thread>(&device::process_outbound_tasks, this);

    m_text_log.debug("Created outbound thread '%i' for device '%s'", m_outbound_thread->get_id(), m_name.c_str());
}


/**
 * Process all outbound tasks in the queue
 */
void device::process_outbound_tasks()
{
    while (!m_exit_outbound_thread.load()) {
        // wait for a task to be added to the queue
        std::shared_ptr<midi_message> msg;
        {
            std::unique_lock<std::mutex> lock(m_outbound_mutex);

            while (m_outbound_msg.empty()) {
                m_new_outbound_task.wait_for(lock, std::chrono::seconds(10));

                if (m_exit_outbound_thread.load()) {
                    m_text_log.debug("Exit outbound thread is enabled (wait loop)");
                    break;
                }
            }

            if (m_exit_outbound_thread.load()) {
                m_text_log.debug("Exit outbound thread is enabled (task loop)");
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
            } catch (const RtMidiError &error) {
                m_text_log.error("MIDI device '%s' :: %s", m_name.c_str(), error.what());
                m_text_log.error("Lost connection to MIDI device, stopped sending outbound messages");

                m_midi_out->closePort();
                m_exit_outbound_thread.store(true);
            }
        }
    }

    m_text_log.debug("Exit outbound thread");
}


/**
 * Add an outbound task to the queue
 */
void device::add_outbound_task(const std::shared_ptr<outbound_task> &in_task)
{
    if (m_exit_outbound_thread.load()) {
        m_text_log.debug("Outbound message rejected, as outbound thread is going to terminate");
        return;
    }

    std::shared_ptr<midi_message> msg = std::make_shared<midi_message>(m_text_log, midi_direction::out);

    msg->set_time(std::chrono::system_clock::now());
    msg->set_port(m_port_out);

    switch (in_task->type) {
        case midi_msg_type::control_change:
            msg->set_status((int) (0xb0 | (in_task->ch - 1)));
            break;

        case midi_msg_type::note_on:
            msg->set_status((int) (0x90 | (in_task->ch - 1)));
            break;

        case midi_msg_type::note_off:
            msg->set_status((int) (0x80 | (in_task->ch - 1)));
            break;

        case midi_msg_type::pitch_bend:
            msg->set_status((int) (0xe0 | (in_task->ch - 1)));
            break;

        case midi_msg_type::program_change:
            msg->set_status((int) (0xc0 | (in_task->ch - 1)));
            break;

        default:
            break;

    }

    msg->set_data_1(in_task->data);
    msg->set_data_2(in_task->velocity);

    msg->add_mapping(in_task->map);

    // add message for internal list, but only if the dataref value has been changed, otherwise the logging will
    // increase very quickly
    if (in_task->data_changed) {
        m_midi_log.add(msg);
        m_text_log.debug("Outbound message for device '%s' on port '%i' :: Status = '%i', Data 1 = '%i', Data 2 = '%i'",
                         m_name.c_str(),
                         m_port_out,
                         msg->status(),
                         msg->data_1(),
                         msg->data_2());
    }

    std::lock_guard<std::mutex> lock(m_outbound_mutex);

    m_outbound_msg.push(msg);
    m_new_outbound_task.notify_one();
}

} // Namespace xmidictrl