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

// X-Plane Environment
#include "PluginLogger.h"

// XMidiCtrl
#include "MappingEncoder.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MappingEncoder::MappingEncoder(int cc)
        : Mapping(cc) {}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
MappingType MappingEncoder::type() {
    return MappingType::Encoder;
};


/**
 * Set the command for up
 */
void MappingEncoder::setCommandUp(std::string_view commandUp) {
    m_commandUp = commandUp;
}


/**
 * Return command for up
 */
std::string_view MappingEncoder::commandUp() const {
    return m_commandUp;
}


/**
 * Set the command for fast up
 */
void MappingEncoder::setCommandFastUp(std::string_view commandFastUp) {
    m_commandFastUp = commandFastUp;
}


/**
 * Return command for fast up
 */
std::string_view MappingEncoder::commandFastUp() const {
    if (m_commandFastUp.empty())
        return m_commandUp;
    else
        return m_commandFastUp;
}


/**
 * Set the command for down
 */
void MappingEncoder::setCommandDown(std::string_view commandDown) {
    m_commandDown = commandDown;
}


/**
 * Return command for down
 */
std::string_view MappingEncoder::commandDown() const {
    return m_commandDown;
}


/**
 * Set the command for fast down
 */
void MappingEncoder::setCommandFastDown(std::string_view commandFastDown) {
    m_commandFastDown = commandFastDown;
}


/**
 * Return command for fast down
 */
std::string_view MappingEncoder::commandFastDown() const {
    if (m_commandFastDown.empty())
        return m_commandDown;
    else
        return m_commandFastDown;
}


/**
 * Check the mapping
 */
bool MappingEncoder::check() {
    if (!Mapping::check())
        return false;

    if (m_commandUp.empty() && m_commandDown.empty() && m_commandFastUp.empty() && m_commandFastDown.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void MappingEncoder::execute(Environment::ptr environment, MidiEvent::ptr midiEvent) {
    LOG_DEBUG << "MappingEncoder::execute" << LOG_END

    if (midiEvent->velocity() < 64) {
        // Down
        if (midiEvent->velocity() < 61)
            environment->commands()->execute(m_commandFastDown);
        else
            environment->commands()->execute(m_commandDown);
    } else if (midiEvent->velocity() > 64) {
        // Up
        if (midiEvent->velocity() > 68)
            environment->commands()->execute(m_commandFastUp);
        else
            environment->commands()->execute(m_commandUp);
    }
}

} // Namespace XMidiCtrl