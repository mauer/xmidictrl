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

// Standard
#include <thread>

// XMidiCtrl
#include "device_list.h"
#include "logger.h"
#include "map_in_pnp.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
device::device(std::string_view name,
               unsigned int port_in,
               unsigned int port_out,
               std::shared_ptr<device_list> device_list)
    : m_device_list(std::move(device_list)), m_name(name), m_port_in(port_in), m_port_out(port_out)
{
    try {
        // create midi classes
        m_midi_in = std::make_unique<RtMidiIn>();
        m_midi_out = std::make_unique<RtMidiOut>();

        LOG_INFO << "Created new MIDI device :: Name = '" << m_name.data() << "', " << "Port In = '" << port_in << "', "
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
            m_midi_in->ignoreTypes(false, true, true);
            m_midi_in->setCallback(&device::midi_callback, this);

            LOG_INFO << "Inbound port '" << m_port_in << "' opened for device '" << m_name << "'" << LOG_END

        } catch (const RtMidiError &error) {
            LOG_ERROR << "Could not open inbound port '" << m_port_in << "' for device '" << m_name << "'" << LOG_END
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
            LOG_ERROR << "Could not open outbound port '" << m_port_in << "' for device '" << m_name << "'" << LOG_END
            LOG_ERROR << error.what() << LOG_END

            return false;
        }
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
            LOG_DEBUG << "Close port '" << m_port_in << "' for device '" << m_name << "'" << LOG_END
            m_midi_in->closePort();
        }
    }

    if (m_midi_out != nullptr) {
        if (m_midi_out->isPortOpen()) {
            LOG_DEBUG << "Close port '" << m_port_out << "' for device '" << m_name << "'" << LOG_END
            m_midi_out->closePort();
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
    if (message->size() > 2) {
        std::shared_ptr<midi_message> msg = std::make_shared<midi_message>();

        msg->time = std::chrono::system_clock::now();

        msg->port = m_port_in;
        msg->type = midi_type::inbound;

        msg->status = static_cast<int>(message->at(0));
        msg->data = static_cast<int>(message->at(1));
        msg->velocity = static_cast<int>(message->at(2));

        // add the midi message to the internal message list (for logging)
        logger::instance().post_midi_message(msg);

        LOG_DEBUG << "Inbound message from device '" << m_name << "' on port '" << m_port_in << "' :: " << "Status = '"
                  << msg->status << "', Data = '" << msg->data << "', Velocity = '" << msg->velocity << "'"
                  << LOG_END

        // check status
        if (msg->status < 176 || msg->status > 191) {
            LOG_ERROR << "Inbound message on port '" << m_port_in << "' :: Invalid status '" << msg->status
                      << "', only Control Change messages are supported" << LOG_END
            return;
        }

        // check data
        if (msg->data < 0 || msg->data > 127) {
            LOG_ERROR << "Inbound message on port '" << m_port_in << "' :: Invalid Control Change '" << msg->data
                      << "', supported values are 0 - 127" << LOG_END
            return;
        }

        // check velocity
        if (msg->velocity < 0 || msg->velocity > 127) {
            LOG_ERROR << "Inbound message on port '" << m_port_in << "' :: Invalid velocity '" << msg->velocity
                      << "', supported values are 0 - 127" << LOG_END
            return;
        }

        // check for a mapping
        auto mappings = m_map_in.get(msg->status - OFFSET_MIDI_CHANNEL_STATUS, msg->data);

        for (auto it = mappings.first; it != mappings.second; it++) {
            auto mapping = it->second;

            LOG_DEBUG << " --> Mapping found" << LOG_END

            bool add_task = false;

            // for push and pull we have to wait until the command has ended
            if (mapping->type() == map_type::push_pull) {
                switch (msg->velocity) {
                    case 127: {
                        std::shared_ptr<map_in_pnp> pnp = std::static_pointer_cast<map_in_pnp>(mapping);
                        pnp->set_time_received();
                        add_task = true;
                        break;
                    }

                    case 0: {
                        // no additional task is required, it's already in the event queue waiting for the
                        // release time point
                        std::shared_ptr<map_in_pnp> pnp = std::static_pointer_cast<map_in_pnp>(mapping);
                        pnp->set_time_released();
                        break;
                    }

                    default:
                        LOG_WARN << "Invalid MIDI status '" << msg->status << "' for a Push&Pull mapping"
                                 << LOG_END
                        break;
                }

            } else if (mapping->type() == map_type::dataref) {
                // for dataref changes, we will only process the event with key pressed (velocity == 127)
                if (msg->velocity == 127)
                    add_task = true;

            } else if (mapping->type() == map_type::command) {
                // lock the current control change for outgoing messages
                if (msg->velocity == 127)
                    m_ch_cc_locked.insert(utils::ch_cc(msg->status - OFFSET_MIDI_CHANNEL_STATUS, msg->data));
                else if (msg->velocity == 0)
                    m_ch_cc_locked.erase(utils::ch_cc(msg->status - OFFSET_MIDI_CHANNEL_STATUS, msg->data));

                add_task = true;
            } else {
                add_task = true;
            }

            // push to event handler
            if (add_task) {
                std::shared_ptr<task> t = std::make_shared<task>();
                t->msg = msg;
                t->map = mapping;

                m_device_list->add_task(t);
            }
        }
    }
}


/**
 * Process all outbound midi mappings
 */
void device::process_outbound_mappings(std::string_view sl_value)
{
    for (auto &mapping: m_map_out) {
        if (m_ch_cc_locked.contains(utils::ch_cc(mapping.second->ch(), mapping.second->cc())))
            continue;

        std::shared_ptr<midi_message> msg = mapping.second->execute(sl_value);

        if (msg == nullptr)
            continue;

        if (msg->data > 0 && m_midi_out != nullptr && m_midi_out->isPortOpen()) {
            msg->time = std::chrono::system_clock::now();
            msg->port = m_port_out;
            msg->type = midi_type::outbound;

            // add message for internal list
            logger::instance().post_midi_message(msg);

            LOG_DEBUG << "Outbound message for device '" << m_name << " on port '" << m_port_out << "' :: "
                      << "Status = '" << msg->status << "', Data = '" << msg->data << "', Velocity = '"
                      << msg->velocity << "'" << LOG_END

            std::vector<unsigned char> midi_out;
            midi_out.push_back(msg->status);
            midi_out.push_back(msg->data);
            midi_out.push_back(msg->velocity);

            try {
                m_midi_out->sendMessage(&midi_out);
            } catch (const RtMidiError &error) {
                LOG_ERROR << "MIDI Device '" << m_name << "' :: " << error.what() << LOG_END;
            }
        }
    }
}


/**
 * Process a reset off all outbound mappings
 */
void device::process_outbound_reset()
{
    for (auto &mapping: m_map_out) {
        std::shared_ptr<midi_message> msg = mapping.second->reset();

        if (msg == nullptr)
            continue;

        if (msg->data > 0 && m_midi_out != nullptr && m_midi_out->isPortOpen()) {
            msg->time = std::chrono::system_clock::now();
            msg->port = m_port_out;
            msg->type = midi_type::outbound;

            // add message for internal list
            logger::instance().post_midi_message(msg);

            LOG_DEBUG << "Outbound message for device '" << m_name << " on port '" << m_port_out << "' :: "
                      << "Status = '" << msg->status << "', Data = '" << msg->data << "', Velocity = '"
                      << msg->velocity << "'" << LOG_END

            std::vector<unsigned char> midi_out;
            midi_out.push_back(msg->status);
            midi_out.push_back(msg->data);
            midi_out.push_back(msg->velocity);

            try {
                m_midi_out->sendMessage(&midi_out);
            } catch (const RtMidiError &error) {
                LOG_ERROR << "MIDI Device '" << m_name << "' :: " << error.what() << LOG_END;
            }
        }
    }
}

} // Namespace xmidictrl