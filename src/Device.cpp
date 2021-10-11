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

// Standard
#include <thread>
#include <utility>

// X-Plane Environment
#include "PluginLogger.h"

// XMidiCtrl
#include "Device.h"
#include "MappingPushAndPull.h"
#include "MidiEvent.h"
#include "Plugin.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Device::Device(std::string_view name, unsigned int portIn, unsigned int portOut) {
    m_name = name;
    m_portIn = portIn;
    m_portOut = portOut;

    try {
        // create midi classes
        m_midiIn = std::make_unique<RtMidiIn>();
        m_midiOut = std::make_unique<RtMidiOut>();

        LOG_INFO << "DEVICE :: Created new MIDI device" << LOG_END
        LOG_INFO << "DEVICE ::  + Name     = " << m_name.data() << LOG_END
        LOG_INFO << "DEVICE ::  + Port In  = " << portIn << LOG_END
        LOG_INFO << "DEVICE ::  + Port Out = " << portOut << LOG_END

    } catch (RtMidiError &error) {
        // we should never reach this, but let's be on the safe side
        LOG_ERROR << "DEVICE :: Error creating midi connections for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}


//Device::Device(DeviceSettings settings) {
//    // set the device settings
//    m_settings = std::move(settings);
//
//    try {
//        // create midi classes
//        m_midiIn  = std::make_unique<RtMidiIn>();
//        m_midiOut = std::make_unique<RtMidiOut>();
//
//        LOG_INFO << "DEVICE :: Created new MIDI device" << LOG_END
//        LOG_INFO << "DEVICE ::  + Name     = " << m_settings.name << LOG_END
//        LOG_INFO << "DEVICE ::  + Port In  = " << m_settings.portIn << LOG_END
//        LOG_INFO << "DEVICE ::  + Port Out = " << m_settings.portOut << LOG_END
//
//    } catch (RtMidiError& error) {
//        // we should never reach this, but let's be on the safe side
//        LOG_ERROR << "DEVICE :: Error creating midi connections for device " << m_settings.name << LOG_END
//        LOG_ERROR << error.what() << LOG_END
//    }
//}


/**
 * Destructor
 */
Device::~Device() {
    LOG_DEBUG << "DEVICE :: Destroying MIDI device '" << m_name.data() << "'" << LOG_END

    // close connections
    closeConnections();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a new mapping to the device
 */
void Device::addMapping(std::shared_ptr<Mapping> mapping) {
    m_mappings.add(mapping);
}


/**
 * Open connections for midi in and out
 */
bool Device::openConnections() {
    // open midi in
    try {
        m_midiIn->openPort(m_portIn);
        m_midiIn->ignoreTypes(false, false, false);
        m_midiIn->setCallback(&Device::midiCallback, this);

        LOG_INFO << "DEVICE :: Port '" << m_portIn << "' opened for device '" << m_name << "'" << LOG_END

    } catch (const RtMidiError &error) {
        LOG_ERROR << "DEVICE :: Could not open port '" << m_portIn << "' for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END

        return false;
    }

    // open midi out
    try {
        m_midiOut->openPort(m_portOut);

        LOG_INFO << "DEVICE :: Port '" << m_portOut << "' opened for device '" << m_name << "'" << LOG_END

    } catch (const RtMidiError &error) {
        LOG_ERROR << "DEVICE :: Could not open port '" << m_portIn << "' for device '" << m_name << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END

        return false;
    }

    // connections successfully established
    return true;
}


/**
 * Close midi connections
 */
void Device::closeConnections() {
    LOG_DEBUG << "DEVICE :: Closing ports for device '" << m_name << "'" << LOG_END

    if (m_midiIn != nullptr)
        m_midiIn->closePort();

    if (m_midiOut != nullptr)
        m_midiOut->closePort();
}


/**
 * Callback method for midi inbound events
 */
void Device::midiCallback(double deltatime, std::vector<unsigned char> *message, void *userdata) {
    Device *midiDevice;

    if (userdata != nullptr) {
        midiDevice = static_cast< Device * >(userdata);
        midiDevice->processMessage(deltatime, message);
    }
}


/**
 * Process an inbound midi message
 */
void Device::processMessage(double deltatime, std::vector<unsigned char> *message) {
    bool addEvent = false;

    // read message
    if (message->size() > 2) {
        // read the midi event
        MidiEvent::ptr midiEvent = std::make_shared<MidiEvent>(static_cast< int >(message->at(1)),
                                                               static_cast< int >(message->at(0)),
                                                               static_cast< int >(message->at(2)));

        LOG_DEBUG << "DEVICE :: Inbound message for device '" << m_name << "' on Thread '" << std::this_thread::get_id()
                  << "' ::  Status = '" << midiEvent->status()
                  << "', CC = '" << midiEvent->controlChange() << "', Velocity = '" << midiEvent->velocity() << "'" << LOG_END

        // check midi status
        if (midiEvent->status() != XMIDICTRL_STATUS_CC) {
            LOG_ERROR << "DEVICE :: Invalid midi status '" << midiEvent->status() << "'" << LOG_END
            return;
        }

        // check for a mapping
        try {
            Mapping::ptr mapping = m_mappings.mappingForControlChange(midiEvent->controlChange());

            // for push and pull we have to wait until the command has ended
            if (mapping->type() == MappingType::PushAndPull) {
                switch (midiEvent->velocity()) {
                    case 127:
                        saveEventDateTime(midiEvent->controlChange());
                        break;

                    case 0: {
                        double seconds = retrieveEventDateTime(midiEvent->controlChange());

                        if (seconds > -0.5f) {
                            MappingPushAndPull_ mappingPnP = std::static_pointer_cast<MappingPushAndPull>(mapping);

                            mappingPnP->setCommandType(seconds < 1 ? CommandType::Push : CommandType::Pull);
                            addEvent = true;
                        }

                        break;
                    }

                    default:
                        LOG_WARN << "DEVICE :: Invalid midi status '" << midiEvent->status() << "'" << LOG_END
                        break;
                }

                // for dataref changes, we will only process the event with key pressed (velocity == 127)
            } else if (mapping->type() == MappingType::Dataref) {
                if (midiEvent->velocity() == 127)
                    addEvent = true;
            } else
                addEvent = true;

            // push to event handler
            if (addEvent) {
                MappedEvent::ptr mappedEvent = std::make_shared<MappedEvent>(midiEvent, mapping);

                Plugin::Instance().addMappedEvent(mappedEvent);
            }

        } catch (std::out_of_range const &) {
            LOG_WARN << "DEVICE :: No mapping found for CC" << midiEvent->controlChange() << LOG_END
        }
    }
}


/**
 * Return the name of the midi device
 */
std::string_view Device::name() const {
    return m_name;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Save the current midi event time stamp for later processing
 */
void Device::saveEventDateTime(int controlChange) {
    m_eventStorage.emplace(controlChange, std::chrono::system_clock::now());
}


/**
 * Retrieve a stored event date time stamp and calculate duration between push and release
 */
double Device::retrieveEventDateTime(int controlChange) {
    try {
        time_point prevTime = m_eventStorage.at(controlChange);
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - prevTime;

        // delete entry
        m_eventStorage.erase(controlChange);

        return elapsed_seconds.count();

    } catch (std::out_of_range const &) {
        return -1;
    }

    return -1;
}

} // Namespace XMidiCtrl