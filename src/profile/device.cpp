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
#include "logger.h"
#include "map_in_cmd.h"
#include "map_in_pnp.h"
#include "plugin.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
device::device(std::string_view name, unsigned int port_in, unsigned int port_out, mode_out mode_out)
    : m_name(name),
      m_port_in(port_in),
      m_port_out(port_out),
      m_mode_out(mode_out)
{
    try {
        // create midi classes
        m_midi_in = std::make_unique<RtMidiIn>();
        m_midi_out = std::make_unique<RtMidiOut>();

        LOG_INFO << "Created new MIDI device :: Name = '" << m_name.data() << "', " << "Port In = '" << port_in << ", "
                 << "Port Out = '" << port_out << "'" << LOG_END

    } catch (RtMidiError &error) {
        // we should never reach this, but let's be on the safe side
        LOG_ERROR << "Error creating MIDI connections for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
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
void device::add_inbound_map(std::shared_ptr<map_in> &mapping)
{
    m_map_in.add(mapping);
}


/**
 * Add a new outbound mapping to the device
 */
void device::add_outbound_map(std::shared_ptr<map_out> &mapping)
{
    m_map_out.add(mapping);
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

            LOG_INFO << "Inbound port '" << m_port_in << "' opened for device '" << m_name << "'" << LOG_END

        } catch (const RtMidiError &error) {
            LOG_ERROR << "Could not open inbound port '" << m_port_in << "' for device '" << m_name << "'"
                      << LOG_END
            LOG_ERROR << error.what() << LOG_END

            return false;
        }
    }

    // open midi out
    if (m_map_out.size() > 0) {
        try {
            m_midi_out->openPort(m_port_out);

            LOG_INFO << "Outbound port '" << m_port_out << "' opened for device '" << m_name << "'" << LOG_END

        } catch (const RtMidiError &error) {
            LOG_ERROR << "Could not open outbound port '" << m_port_in << "' for device '" << m_name << "'"
                      << LOG_END
            LOG_ERROR << error.what() << LOG_END

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

            LOG_INFO << "Closed port '" << m_port_in << "' for device '" << m_name << "'" << LOG_END
        }
    }

    if (m_outbound_thread != nullptr) {
        LOG_DEBUG << "Terminate outbound thread '" << m_outbound_thread->get_id() << "' for device '" << m_name
                  << "'"
                  << LOG_END

        m_exit_outbound_thread.store(true);
        m_new_outbound_task.notify_one();

        m_outbound_thread->join();
        m_outbound_thread = nullptr;
    }

    if (m_midi_out != nullptr) {
        if (m_midi_out->isPortOpen()) {
            m_midi_out->closePort();

            LOG_INFO << "Closed port '" << m_port_out << "' for device '" << m_name << "'" << LOG_END
        }
    }
}


/**
 * Callback method for midi inbound events
 */
void device::midi_callback(double deltatime, std::vector<unsigned char> *message, void *userdata)
{
    if (userdata != nullptr) {
        auto *dev = static_cast<device *>(userdata);
        dev->process_inbound_message(deltatime, message);
    }
}


/**
 * Process an inbound midi message
 */
void device::process_inbound_message(double deltatime, std::vector<unsigned char> *message)
{
    // read midi message
    if (message->size() > 1) {
        std::shared_ptr<midi_message> msg = std::make_shared<midi_message>();

        // create an internal midi message including all data received
        msg->time = std::chrono::system_clock::now();

        msg->port = m_port_in;
        msg->direction = msg_direction::inbound;

        msg->status = message->at(0);
        msg->data = message->at(1);

        if (message->size() > 2)
            msg->velocity = message->at(2);

        // add the midi message to the internal message list (for logging)
        logger::instance().post_midi_message(msg);

        LOG_DEBUG << "Inbound message from device '" << m_name << "' on port '" << m_port_in << "' :: "
                  << "Status = '" << msg->status << "', Data = '" << msg->data << "', Data Velocity = '"
                  << msg->velocity << "'" << LOG_END

        // check midi type
        switch (msg->get_type()) {
            case midi_msg_type::aftertouch: {
                LOG_ERROR << "Inbound message on port '" << m_port_in << "' :: Invalid MIDI type" << LOG_END
                LOG_ERROR << " --> 'Aftertouch' messages are not supported" << LOG_END
                return;
            }

            case midi_msg_type::channel_pressure: {
                LOG_ERROR << "Inbound message on port '" << m_port_in << "' :: Invalid MIDI type" << LOG_END
                LOG_ERROR << " --> 'Channel Pressure' messages are not supported" << LOG_END
                return;
            }

            case midi_msg_type::control_change:
            case midi_msg_type::note_off:
            case midi_msg_type::note_on:
            case midi_msg_type::pitch_bend:
            case midi_msg_type::program_change:
                break;

            case midi_msg_type::none: {
                return;
            }
        }

        // check for a mapping
        auto mappings = m_map_in.get(msg->get_key());

        for (auto it = mappings.first; it != mappings.second; it++) {
            auto mapping = it->second;

            LOG_DEBUG << " --> Mapping found" << LOG_END

            bool add_task = false;

            // for push and pull we have to wait until the command has ended
            if (mapping->type() == map_type::push_pull) {
                // TODO: Move into mapping object (e.g. set_times)
                switch (msg->velocity) {
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
                        LOG_WARN << "Invalid MIDI velocity '" << msg->velocity << "' for a Push&Pull mapping"
                                 << LOG_END
                        break;
                }

            } else if (mapping->type() == map_type::dataref) {
                // for dataref changes, we will only process the event with key pressed (velocity == 127)
                if (msg->velocity == MIDI_VELOCITY_MAX)
                    add_task = true;

            } else if (mapping->type() == map_type::command) {
                std::shared_ptr<map_in_cmd> cmd = std::static_pointer_cast<map_in_cmd>(mapping);

                // lock the current control change for outgoing messages
                // TODO: Lock it for other messages types, as well
                if (msg->velocity == cmd->velocity_on())
                    m_ch_cc_locked.insert(msg->get_key());
                else if (msg->velocity == cmd->velocity_off())
                    m_ch_cc_locked.erase(msg->get_key());

                add_task = true;
            } else {
                add_task = true;
            }

            // push to event handler
            if (add_task) {
                std::shared_ptr<inbound_task> t = std::make_shared<inbound_task>();
                t->msg = msg;
                t->map = mapping;

                plugin::instance().add_inbound_task(t);
            }
        }
    }
}


/**
 * Process all outbound midi mappings
 */
void device::process_outbound_mappings()
{
    if (m_mode_out == mode_out::permanent) {
        if (m_time_sent != time_point::min()) {
            std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - m_time_sent;

            if (elapsed_seconds.count() < 0.5f)
                return;
        }
    }

    for (auto &mapping: m_map_out) {
        if (m_ch_cc_locked.contains(mapping->get_key())) {
            LOG_DEBUG << "Locked: " << mapping->get_key() << LOG_END
            continue;
        }

        std::shared_ptr<outbound_task> task = mapping->execute(m_mode_out);

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

    LOG_DEBUG << "Created outbound thread '" << m_outbound_thread->get_id() << "' for device '" << m_name << "'"
              << LOG_END
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
                    LOG_DEBUG << "Exit outbound thread is enabled (wait loop)" << LOG_END
                    break;
                }
            }

            if (m_exit_outbound_thread.load()) {
                LOG_DEBUG << "Exit outbound thread is enabled (task loop)" << LOG_END
                continue;
            }

            if (m_outbound_msg.empty())
                continue;

            msg = m_outbound_msg.front();
            m_outbound_msg.pop();
        }

        if (msg != nullptr) {
            std::vector<unsigned char> midi_out;
            midi_out.push_back(msg->status);
            midi_out.push_back(msg->data);
            midi_out.push_back(msg->velocity);

            try {
                m_midi_out->sendMessage(&midi_out);
            } catch (const RtMidiError &error) {
                LOG_ERROR << "MIDI device '" << m_name << "' :: " << error.what() << LOG_END;

                // stop sending outbound messages
                LOG_ERROR << "Connection to MIDI device got lost, stopped sending outbound messages" << LOG_END

                m_midi_out->closePort();
                m_exit_outbound_thread.store(true);
            }
        }
    }

    LOG_DEBUG << "Exit outbound thread" << LOG_END
}


/**
 * Add an outbound task to the queue
 */
void device::add_outbound_task(const std::shared_ptr<outbound_task> &task)
{
    if (m_exit_outbound_thread.load()) {
        LOG_DEBUG << "Outbound message rejected, as outbound thread is going to terminate" << LOG_END
        return;
    }

    std::shared_ptr<midi_message> msg = std::make_shared<midi_message>();

    msg->time = std::chrono::system_clock::now();
    msg->port = m_port_out;
    msg->direction = msg_direction::outbound;

    switch (task->type) {
        case midi_msg_type::control_change:
            msg->status = (int) (0xb0 | (task->ch - 1));
            break;

        case midi_msg_type::note_on:
            msg->status = (int) (0x90 | (task->ch - 1));
            break;

        case midi_msg_type::note_off:
            msg->status = (int) (0x80 | (task->ch - 1));
            break;

        case midi_msg_type::pitch_bend:
            msg->status = (int) (0xe0 | (task->ch - 1));
            break;

        case midi_msg_type::program_change:
            msg->status = (int) (0xc0 | (task->ch - 1));
            break;

        default:
            break;

    }

    msg->data = task->data;
    msg->velocity = task->velocity;

    // add message for internal list, but only if the dataref value has been changed, otherwise the logging will
    // increase very quickly
    if (task->data_changed) {
        logger::instance().post_midi_message(msg);

        LOG_DEBUG << "Outbound message for device '" << m_name << " on port '" << m_port_out << "' :: "
                  << "Status = '" << msg->status << "', Data 1 = '" << msg->data << "', Data 2 = '"
                  << msg->velocity << "'" << LOG_END
    }

    std::lock_guard<std::mutex> lock(m_outbound_mutex);

    m_outbound_msg.push(msg);
    m_new_outbound_task.notify_one();
}

} // Namespace xmidictrl