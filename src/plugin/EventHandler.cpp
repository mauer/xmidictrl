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
#include <mutex>
#include <string>
#include <thread>
#include <utility>

// XMidiCtrl
#include "EventHandler.h"
#include "Logger.h"
#include "Mapping.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
EventHandler::EventHandler(Environment::ptr environment) {
    m_environment = std::move(environment);
}


/**
 * Destructor
 */
EventHandler::~EventHandler() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a midi event
 */
void EventHandler::addMappedEvent(const MappedEvent::ptr& mappedEvent) {
    LOG_DEBUG << "EVENTHANDLER :: Adding event on Thread " << std::this_thread::get_id() << " :: Status = '"
              << mappedEvent->midiEvent()->status()
              << "', CC = '" << mappedEvent->midiEvent()->controlChange() << "', Velocity = '"
              << mappedEvent->midiEvent()->velocity() << "'" << LOG_END

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    m_eventList.push(mappedEvent);
}


/**
 * Process events
 */
void EventHandler::processEvents() {
    MappedEvent::ptr mappedEvent;

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    // process the midi inbound queue for each midi device
    while (!m_eventList.empty()) {
        mappedEvent = m_eventList.front();

        if (mappedEvent == nullptr)
            continue;

        // perform the action related to the mapping
        if (mappedEvent->mapping())
            mappedEvent->mapping()->execute(mappedEvent->midiEvent());

        // delete entry from list
        m_eventList.pop();
    }
}

/**
 * Change an float dataref
 */
//void EventHandler::changeFloatDataRef(const DataDetails& dataDetails, const std::shared_ptr<MidiEvent>& midiEvent) {
//    float value = XPLMGetDataf(dataDetails.dataRef);
//
//    if (value == std::stof(midiEvent->mapping.valueOn)) {
//        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOff << LOG_END
//        XPLMSetDataf(dataDetails.dataRef, std::stof(midiEvent->mapping.valueOff));
//    } else {
//        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
//        XPLMSetDataf(dataDetails.dataRef, std::stof(midiEvent->mapping.valueOn));
//    }
//}


/**
 * Change a double dataref
 */
//void EventHandler::changeDoubleDataRef(const DataDetails& dataDetails, const std::shared_ptr<MidiEvent>& midiEvent) {
//    double value = XPLMGetDatad(dataDetails.dataRef);
//
//    if (value == std::stod(midiEvent->mapping.valueOn)) {
//        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOff << LOG_END
//        XPLMSetDatad(dataDetails.dataRef, std::stod(midiEvent->mapping.valueOff));
//    } else {
//        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
//        XPLMSetDatad(dataDetails.dataRef, std::stod(midiEvent->mapping.valueOn));
//    }
//}

} // Namespace XMidiCtrl