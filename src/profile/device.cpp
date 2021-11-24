//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and 
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of 
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//   IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#include "device.h"

// Standard
#include <thread>

// XMidiCtrl
#include "device_list.h"
#include "logger.h"
#include "map_in_pnp.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
device::device(std::string_view name, unsigned int port_in, unsigned int port_out,
               std::shared_ptr<device_list> device_list)
    : m_device_list(std::move(device_list)),
      m_name(name),
      m_port_in(port_in),
      m_port_out(port_out)
{
    try {
        // create midi classes
        m_midi_in = std::make_unique<RtMidiIn>();
        m_midi_out = std::make_unique<RtMidiOut>();

        LOG_INFO << "Created new MIDI device :: Name = '" << m_name.data() << "', "
                 << "Port In = '" << port_in << "', " << "Port Out = '" << port_out << "'" << LOG_END

    } catch (RtMidiError &error) {
        // we should never reach this, but let's be on the safe side
        LOG_ERROR << "Error creating midi connections for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}


/**
 * Destructor
 */
device::~device()
{
    // close connections
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
    try {
        m_midi_in->openPort(m_port_in);
        m_midi_in->ignoreTypes(false, false, false);
        m_midi_in->setCallback(&device::midi_callback, this);

        LOG_INFO << "Inbound port '" << m_port_in << "' opened for device '" << m_name << "'" << LOG_END

    } catch (const RtMidiError &error) {
        LOG_ERROR << "Could not open inbound port '" << m_port_in << "' for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END

        return false;
    }

    // open midi out
    try {
        m_midi_out->openPort(m_port_out);

        LOG_INFO << "Outbound port '" << m_port_out << "' opened for device '" << m_name << "'" << LOG_END

    } catch (const RtMidiError &error) {
        LOG_ERROR << "Could not open outbound port '" << m_port_in << "' for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END

        return false;
    }

    // connections successfully established
    return true;
}


/**
 * Close midi connections
 */
void device::close_connections()
{
    LOG_DEBUG << "Closing ports for device '" << m_name << "'" << LOG_END

    if (m_midi_in != nullptr)
        m_midi_in->closePort();

    if (m_midi_out != nullptr)
        m_midi_out->closePort();
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
    bool add_event = false;

    // read message
    if (message->size() > 2) {
        // read the midi event
        midi_message::ptr midi = std::make_shared<midi_message>();

        midi->cc = static_cast<int>(message->at(1));
        midi->status = static_cast<int>(message->at(0));
        midi->velocity = static_cast<int>(message->at(2));

        LOG_DEBUG << "Inbound message from device '" << m_name << "' on Thread '" << std::this_thread::get_id()
                  << "' ::  Status = '" << midi->status << "', "
                  << "CC = '" << midi->cc << "', "
                  << "Velocity = '" << midi->velocity << "'" << LOG_END

        // check midi status
        if (midi->status != static_cast<int>(midi_status::control_change)) {
            LOG_ERROR << "Invalid midi status '" << midi->status << "' in message from device '"
                      << m_name << "'" << LOG_END
            return;
        }

        // check for a mapping
        try {
            auto mappings = m_map_in.mappingForControlChange(midi->cc);

            for (auto it = mappings.first; it != mappings.second; it++) {
                auto mapping = it->second;

                // for push and pull we have to wait until the command has ended
                if (mapping->type() == mapping_type::push_pull) {
                    switch (midi->velocity) {
                        case 127: {
                            save_event_datetime(midi->cc);
                            break;
                        }

                        case 0: {
                            double seconds = retrieve_event_datetime(midi->cc);

                            if (seconds > -0.5f) {
                                map_in_pnp::ptr
                                    mappingPnP = std::static_pointer_cast<map_in_pnp>(mapping);

                                mappingPnP->set_command_type(seconds < 1 ? CommandType::Push : CommandType::Pull);
                                add_event = true;
                            }

                            break;
                        }

                        default:
                            LOG_WARN << "Invalid midi status '" << midi->status << "' for a Push & Pull mapping"
                                     << LOG_END
                            break;
                    }

                    // for dataref changes, we will only process the event with key pressed (velocity == 127)
                } else if (mapping->type() == mapping_type::dataref) {
                    if (midi->velocity == 127)
                        add_event = true;
                } else if (mapping->type() == mapping_type::command) {
                    // lock the current control change for outgoing messages
                    if (midi->velocity == 127)
                        m_cc_locked.insert(midi->cc);
                    else if (midi->velocity == 0)
                        m_cc_locked.erase(midi->cc);

                    add_event = true;
                } else
                    add_event = true;

                // push to event handler
                if (add_event) {
                    std::shared_ptr<inbound_event> event = std::make_shared<inbound_event>();
                    event->event = midi;
                    event->map = mapping;

                    m_device_list->add_event(event);
                }
            }
        } catch (std::out_of_range const &) {
            LOG_WARN << "No mapping found for CC '" << midi->cc << "' midi message from device '"
                     << m_name << "'" << LOG_END
        }
    }
}


/**
 * Process all outbound midi mappings
 */
void device::process_outbound_mappings()
{
    for (auto &mapping: m_map_out) {
        if (m_cc_locked.contains(mapping.second->cc()))
            continue;

        midi_message::ptr message = mapping.second->execute();

        if (message == nullptr)
            continue;

        if (message->cc > 0 && m_midi_out != nullptr && m_midi_out->isPortOpen()) {
            LOG_DEBUG << "Outbound message for device '" << m_name
                      << "' ::  CC = '" << message->cc << "', "
                      << "Status = '" << message->status << "', "
                      << "Velocity = '" << message->velocity << "'" << LOG_END

            std::vector<unsigned char> midi_out;
            midi_out.push_back(message->status);
            midi_out.push_back(message->cc);
            midi_out.push_back(message->velocity);

            try {
                m_midi_out->sendMessage(&midi_out);
            } catch (const RtMidiError &error) {
                LOG_ERROR << "Midi Device '" << m_name << "' :: " << error.what() << LOG_END;
            }
        }
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Save the current midi event time stamp for later processing
 */
void device::save_event_datetime(unsigned int cc)
{
    m_event_storage.emplace(cc, std::chrono::system_clock::now());
}


/**
 * Retrieve a stored event date time stamp and calculate duration between push and release
 */
double device::retrieve_event_datetime(unsigned int cc)
{
    try {
        time_point prevTime = m_event_storage.at(cc);
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - prevTime;

        // delete entry
        m_event_storage.erase(cc);

        return elapsed_seconds.count();

    } catch (std::out_of_range const &) {
        return -1;
    }
}

} // Namespace XMidiCtrl