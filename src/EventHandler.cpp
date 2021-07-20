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

// X-Plane SDK Utils
#include "PluginLogger.h"

// XMidiCtrl
#include "EventHandler.h"


using namespace XMidiCtrl;


//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
EventHandler::EventHandler(std::shared_ptr<Environment> environment) {
    m_environment = std::move(environment);
}


/**
 * Destructor
 */
EventHandler::~EventHandler() {
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a midi event
 */
void EventHandler::addMidiEvent(std::shared_ptr<MidiEvent> midiEvent) {
    LOG_DEBUG << "EVENTHANDLER :: Adding midiEvent on Thread " << std::this_thread::get_id() << " :: Status = " << midiEvent->status 
              << ", CC = " << midiEvent->controlChange << ", Velocity = " << midiEvent->velocity << LOG_END
    
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    m_eventList.push(midiEvent);
}


/**
 * Process events
 */
void EventHandler::processEvents() {
    std::shared_ptr<MidiEvent> midiEvent;

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    // process the midi inbound queue for each midi device
    while (!m_eventList.empty()) {
        midiEvent = m_eventList.front();

        if (midiEvent == nullptr)
            continue;

        switch (midiEvent->mapping.type) {
            case MappingType::None:
                break;

            case MappingType::Command:
                LOG_DEBUG << "EVENTHANDLER :: Process Command event on Thread " << std::this_thread::get_id() << LOG_END
                executeCommand(midiEvent);

                break;

            case MappingType::DataRef:
                LOG_DEBUG << "EVENTHANDLER :: Process DataRef event on Thread " << std::this_thread::get_id() << LOG_END
                changeDataRef(midiEvent);

                break;

            case MappingType::Slider:
                LOG_DEBUG << "EVENTHANDLER :: Process Slider event on Thread " << std::this_thread::get_id() << LOG_END
                performSliderEvent(midiEvent);

                break;

            case MappingType::PushAndPull:
                LOG_DEBUG << "EVENTHANDLER :: Process Push and Pull event on Thread " << std::this_thread::get_id() << LOG_END
                executeCommandOnce(midiEvent);

                break;        

            case MappingType::Encoder:
                LOG_DEBUG << "EVENTHANDLER :: Process Enconder event on Thread " << std::this_thread::get_id() << LOG_END
                performEncoderEvent(midiEvent);

                break;

            case MappingType::Internal:
                LOG_DEBUG << "EVENTHANDLER :: Process Internal event on Thread " << std::this_thread::get_id() << LOG_END
                performInternalEvent(midiEvent);

                break;
        }

        // delete entry from list
        m_eventList.pop();
    }
}


/**
 * Execute a X-Plane command
 */
void EventHandler::executeCommand(std::shared_ptr<MidiEvent> midiEvent) {
    switch (midiEvent->velocity) {
        case 127:
            LOG_DEBUG << "Execute begin command " << midiEvent->mapping.command << LOG_END
            m_environment->commands()->begin(midiEvent->mapping.command);
            break;

        case 0:
            LOG_DEBUG << "Execute end command " << midiEvent->mapping.command << LOG_END
            m_environment->commands()->end(midiEvent->mapping.command);
            break;

        default:
            LOG_ERROR << "Invalid Midi status " << midiEvent->status << LOG_END
    }
}


/**
 * Execute a X-Plane command once
 */
void EventHandler::executeCommandOnce(std::shared_ptr<MidiEvent> midiEvent) {
    m_environment->commands()->execute(midiEvent->mapping.command);
}


/**
 * Perform a slider event
 */
void EventHandler::performSliderEvent(std::shared_ptr<MidiEvent> midiEvent) {
    if (midiEvent->velocity <= 10) 
        m_environment->commands()->execute(midiEvent->mapping.commandDown);
    else if (midiEvent->velocity >= 117) 
        m_environment->commands()->execute(midiEvent->mapping.commandUp);
}


/**
 * Perform an encoder event
 */
void EventHandler::performEncoderEvent(std::shared_ptr<MidiEvent> midiEvent) {
    if (midiEvent->velocity < 64) {
        // Down
        if (midiEvent->velocity < 61) 
            m_environment->commands()->execute(midiEvent->mapping.commandDownFast);
        else
            m_environment->commands()->execute(midiEvent->mapping.commandDown);
    } else if (midiEvent->velocity > 64) {
        // Up
        if (midiEvent->velocity > 68)
            m_environment->commands()->execute(midiEvent->mapping.commandUpFast);
        else
            m_environment->commands()->execute(midiEvent->mapping.commandUp);
    }
}


/**
 * Perform an internal event
 */
void EventHandler::performInternalEvent(std::shared_ptr<MidiEvent> midiEvent) {

}


/**
 * Change a dataref directly without using a command
 */
void EventHandler::changeDataRef(std::shared_ptr<MidiEvent> midiEvent) {
    DataDetails data = getDataDetails(midiEvent->mapping.dataRef);

    switch (data.type) {
        case xplmType_Int:
            LOG_DEBUG << "EVENTHANDLER :: Change integer dataref" << LOG_END
            changeIntegerDataRef(data, midiEvent);

            break;

        case xplmType_Float:
            LOG_DEBUG << "EVENTHANDLER :: Change float dataref" << LOG_END
            changeFloatDataRef(data, midiEvent);

            break;

        case xplmType_Double:
            LOG_DEBUG << "EVENTHANDLER :: Change double dataref" << LOG_END

            break;

        case xplmType_Unknown:
            LOG_ERROR << "EVENTHANDLER :: Could not determine type of dataref " << midiEvent->mapping.dataRef << LOG_END

            break;

        default:
            LOG_ERROR << "EVENTHANDLER :: Unkown data type " << data.type << LOG_END
    }
}


/**
 * Change a integer dataref
 */
void EventHandler::changeIntegerDataRef(const DataDetails& dataDetails, std::shared_ptr<MidiEvent> midiEvent) {
    int value = XPLMGetDatai(dataDetails.dataRef);

    if (value == std::stoi(midiEvent->mapping.valueOn)) {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOff << LOG_END
        XPLMSetDatai(dataDetails.dataRef, std::stoi(midiEvent->mapping.valueOff));
    } else {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
        XPLMSetDatai(dataDetails.dataRef, std::stoi(midiEvent->mapping.valueOn));
    }
}


/**
 * Change an float dataref
 */
void EventHandler::changeFloatDataRef(const DataDetails& dataDetails, std::shared_ptr<MidiEvent> midiEvent) {
    float value = XPLMGetDataf(dataDetails.dataRef);

    if (value == std::stof(midiEvent->mapping.valueOn)) {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOff << LOG_END
        XPLMSetDataf(dataDetails.dataRef, std::stof(midiEvent->mapping.valueOff));
    } else {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
        XPLMSetDataf(dataDetails.dataRef, std::stof(midiEvent->mapping.valueOn));
    }
}


/**
 * Change a double dataref
 */
void EventHandler::changeDoubleDataRef(const DataDetails& dataDetails, std::shared_ptr<MidiEvent> midiEvent) {
    double value = XPLMGetDatad(dataDetails.dataRef);

    if (value == std::stod(midiEvent->mapping.valueOn)) {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOff << LOG_END
        XPLMSetDatad(dataDetails.dataRef, std::stod(midiEvent->mapping.valueOff));
    } else {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
        XPLMSetDatad(dataDetails.dataRef, std::stod(midiEvent->mapping.valueOn));
    }
}


/**
 * Get details for X-Plane data
 */
DataDetails EventHandler::getDataDetails(const std::string name) {
    DataDetails data;

    // check the cache first
    try {
        data = m_dataCache.at(name);
    } catch (std::out_of_range const&) {
        data.dataRef = XPLMFindDataRef(name.c_str());
        
        if (data.dataRef != nullptr)
            data.type = XPLMGetDataRefTypes(data.dataRef);
        else
            data.type = xplmType_Unknown;

        m_dataCache.emplace(name, data);
    }

    if (data.dataRef == nullptr)
        LOG_ERROR << "DataRef " << name << " not found" << LOG_END

    return data;
}