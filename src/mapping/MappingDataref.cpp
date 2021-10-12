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
#include "utils/Logger.h"

// XMidiCtrl
#include "MappingDataref.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MappingDataref::MappingDataref(int cc)
        : Mapping(cc) {}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
MappingType MappingDataref::type() {
    return MappingType::Dataref;
};


/**
 * Set the dataref
 */
void MappingDataref::setDataref(std::string_view dataref) {
    m_dataref = dataref;
}


/**
 * Return the dataref
 */
std::string_view MappingDataref::dataref() const {
    return m_dataref;
}


/**
 * Set value on
 */
void MappingDataref::setValueOn(std::string_view valueOn) {
    m_valueOn = valueOn;
}


/**
 * Return value on
 */
std::string_view MappingDataref::valueOn() const {
    return m_valueOn;
}


/**
 * Set value off
 */
void MappingDataref::setValueOff(std::string_view valueOff) {
    m_valueOff = valueOff;
}


/**
 * Return value off
 */
std::string_view MappingDataref::valueOff() const {
    return m_valueOff;
}


/**
 * Check the mapping
 */
bool MappingDataref::check() {
    if (!Mapping::check())
        return false;

    if (m_dataref.empty() && m_valueOn.empty() && m_valueOff.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void MappingDataref::execute(Environment::ptr environment, MidiEvent::ptr midiEvent) {
    LOG_DEBUG << "MappingDataref::execute" << LOG_END

    environment->datarefs()->toggle(m_dataref, m_valueOn, m_valueOff);
}

} // Namespace XMidiCtrl