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
#include <string>

// X-Plane SDK Utils
#include "PluginLogger.h"

// XMidiCtrl
#include "EventHandler.h"


using namespace XMidiCtrl;


//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESCTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
EventHandler::EventHandler() {
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
void EventHandler::addMidiEvent(const MidiEvent midiEvent) {
    m_eventList.push_back(midiEvent);
}


/**
 * Process events
 */
void EventHandler::processEvents() {

    // process the midi inbound queue for each midi device
    for (auto const& midiEvent : m_eventList) {
        switch (midiEvent.mapping.type) {
            case MappingType::None:
                break;

            case MappingType::Command:
                LOG_DEBUG << "EVENTHANDLER :: Process Command event" << LOG_END
                executeCommand(midiEvent);

                break;

            case MappingType::DataRef:
                LOG_DEBUG << "EVENTHANDLER :: Process DataRef event" << LOG_END
                changeDataRef(midiEvent);

                break;

            case MappingType::Slider:
                LOG_DEBUG << "EVENTHANDLER :: Process Slider event" << LOG_END
                performSliderEvent(midiEvent);

                break;

            case MappingType::PushAndPull:
                LOG_DEBUG << "EVENTHANDLER :: Process Push and Pull event" << LOG_END
                executeCommandOnce(midiEvent);

                break;        

            case MappingType::Encoder:
                LOG_DEBUG << "EVENTHANDLER :: Process Enconder event" << LOG_END
                performEncoderEvent(midiEvent);

                break;

            case MappingType::Internal:
                LOG_DEBUG << "EVENTHANDLER :: Process Internal event" << LOG_END

                break;
        }
    }

    // clear all events
    m_eventList.clear();
}


/**
 * Execute a X-Plane command
 */
void EventHandler::executeCommand(const MidiEvent& midiEvent) {
    XPLMCommandRef cmdRef = getCommandRef(midiEvent.mapping.command);

    if (cmdRef == nullptr)
        return;
    
    switch (midiEvent.velocity) {
        case 127:
            LOG_DEBUG << "Execute begin command " << midiEvent.mapping.command << LOG_END
            XPLMCommandBegin(cmdRef);
            break;

        case 0:
            LOG_DEBUG << "Execute end command " << midiEvent.mapping.command << LOG_END
            XPLMCommandEnd(cmdRef);
            break;

        default:
            LOG_ERROR << "Invalid Midi status " << midiEvent.status << LOG_END
    }
}


/**
 * Execute a X-Plane command once
 */
void EventHandler::executeCommandOnce(const MidiEvent& midiEvent) {
    XPLMCommandRef cmdRef = getCommandRef(midiEvent.mapping.command);

    if (cmdRef == nullptr) 
        return;
    

    LOG_DEBUG << "Execute command once " << midiEvent.mapping.command << LOG_END
    XPLMCommandOnce(cmdRef);
}


/**
 * Perform a slider event
 */
void EventHandler::performSliderEvent(const MidiEvent& midiEvent) {
    XPLMCommandRef cmdRef;

    if (midiEvent.velocity <= 10) {
        cmdRef = getCommandRef(midiEvent.mapping.commandDown);

        if (cmdRef == nullptr)
            return;

        LOG_DEBUG << "Execute command once " << midiEvent.mapping.commandDown << LOG_END
        XPLMCommandOnce(cmdRef);
    
    } else if (midiEvent.velocity >= 117) {
        cmdRef = getCommandRef(midiEvent.mapping.commandUp);

        if (cmdRef == nullptr)
            return;  

        LOG_DEBUG << "Execute command once " << midiEvent.mapping.commandUp << LOG_END
        XPLMCommandOnce(cmdRef);
    }
}


/**
 * Perform an encoder event
 */
void EventHandler::performEncoderEvent(const MidiEvent& midiEvent) {
    XPLMCommandRef cmdRef;

    if (midiEvent.velocity < 64) {
        // Down
        if (midiEvent.velocity < 61) {
            cmdRef = getCommandRef(midiEvent.mapping.commandDownFast);

            if (cmdRef == nullptr)
                return;

            LOG_DEBUG << "Execute command once " << midiEvent.mapping.commandDownFast << LOG_END
            XPLMCommandOnce(cmdRef);
        } else {
            cmdRef = getCommandRef(midiEvent.mapping.commandDown);

            if (cmdRef == nullptr)
                return;

            LOG_DEBUG << "Execute command once " << midiEvent.mapping.commandDown << LOG_END
            XPLMCommandOnce(cmdRef);
        }
    } else if (midiEvent.velocity > 64) {
        // Up
        if (midiEvent.velocity > 68) {
            cmdRef = getCommandRef(midiEvent.mapping.commandUpFast);

            if (cmdRef == nullptr)
                return;        

            LOG_DEBUG << "Execute command once " << midiEvent.mapping.commandUpFast << LOG_END
            XPLMCommandOnce(cmdRef);
        } else {
            cmdRef = getCommandRef(midiEvent.mapping.commandUp);

            if (cmdRef == nullptr)
                return;  

            LOG_DEBUG << "Execute command once " << midiEvent.mapping.commandUp << LOG_END
            XPLMCommandOnce(cmdRef);
        }
    }
}


/**
 * Change a dataref directly without using a command
 */
void EventHandler::changeDataRef(const MidiEvent& midiEvent) {
    DataDetails data = getDataDetails(midiEvent.mapping.dataRef);

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
            LOG_ERROR << "EVENTHANDLER :: Could not determine type of dataref " << midiEvent.mapping.dataRef << LOG_END

            break;

        default:
            LOG_ERROR << "EVENTHANDLER :: Unkown data type " << data.type << LOG_END
    }
}


/**
 * Change a integer dataref
 */
void EventHandler::changeIntegerDataRef(const DataDetails& dataDetails, const MidiEvent& midiEvent) {
    int value = XPLMGetDatai(dataDetails.dataRef);

    if (value == std::stoi(midiEvent.mapping.valueOn)) {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent.mapping.dataRef << " to value " << midiEvent.mapping.valueOff << LOG_END
        XPLMSetDatai(dataDetails.dataRef, std::stoi(midiEvent.mapping.valueOff));
    } else {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent.mapping.dataRef << " to value " << midiEvent.mapping.valueOn << LOG_END
        XPLMSetDatai(dataDetails.dataRef, std::stoi(midiEvent.mapping.valueOn));
    }
}


/**
 * Change an float dataref
 */
void EventHandler::changeFloatDataRef(const DataDetails& dataDetails, const MidiEvent& midiEvent) {
    float value = XPLMGetDataf(dataDetails.dataRef);

    if (value == std::stof(midiEvent.mapping.valueOn)) {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent.mapping.dataRef << " to value " << midiEvent.mapping.valueOff << LOG_END
        XPLMSetDataf(dataDetails.dataRef, std::stof(midiEvent.mapping.valueOff));
    } else {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent.mapping.dataRef << " to value " << midiEvent.mapping.valueOn << LOG_END
        XPLMSetDataf(dataDetails.dataRef, std::stof(midiEvent.mapping.valueOn));
    }
}


/**
 * Change a double dataref
 */
void EventHandler::changeDoubleDataRef(const DataDetails& dataDetails, const MidiEvent& midiEvent) {
    double value = XPLMGetDatad(dataDetails.dataRef);

    if (value == std::stod(midiEvent.mapping.valueOn)) {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent.mapping.dataRef << " to value " << midiEvent.mapping.valueOff << LOG_END
        XPLMSetDatad(dataDetails.dataRef, std::stod(midiEvent.mapping.valueOff));
    } else {
        LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent.mapping.dataRef << " to value " << midiEvent.mapping.valueOn << LOG_END
        XPLMSetDatad(dataDetails.dataRef, std::stod(midiEvent.mapping.valueOn));
    }
}


/**
 * Get the command ref for a command string
 */
XPLMCommandRef EventHandler::getCommandRef(const std::string command) {
    XPLMCommandRef cmdRef = nullptr;

    // check the cache first
    try {
        cmdRef = m_commandCache.at(command);
    } catch (std::out_of_range const&) {
        cmdRef = XPLMFindCommand(command.c_str());
        m_commandCache.emplace(command, cmdRef);
    }

    if (cmdRef == nullptr)
        LOG_ERROR << "Command " << command << " not found" << LOG_END

    return cmdRef;
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