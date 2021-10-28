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

// XMidiCtrl
#include "Logger.h"
#include "MappingSlider.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MappingSlider::MappingSlider(XPlane::ptr xplane, int controlChange)
        : Mapping(std::move(xplane), controlChange) {}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
MappingType MappingSlider::type() {
    return MappingType::Slider;
};


/**
 * Set the up command
 */
void MappingSlider::setCommandUp(std::string_view commandUp) {
    m_commandUp = commandUp;
}


/**
 * Return the up command
 */
std::string_view MappingSlider::commandUp() const {
    return m_commandUp;
}


/**
 * Set the down command
 */
void MappingSlider::setCommandDown(std::string_view commandDown) {
    m_commandDown = commandDown;
}


/**
 * Return the down command
 */
std::string_view MappingSlider::commandDown() const {
    return m_commandDown;
}


/**
 * Check the mapping
 */
bool MappingSlider::check() {
    if (!Mapping::check())
        return false;

    if (m_commandUp.empty() && m_commandDown.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void MappingSlider::execute(MidiEvent::ptr midiEvent) {
    LOG_DEBUG << "MappingSlider::execute" << LOG_END

    if (midiEvent->velocity() <= 10)
        m_xplane->commands()->execute(m_commandDown);
    else if (midiEvent->velocity() >= 117)
        m_xplane->commands()->execute(m_commandUp);
}

} // Namespace XMidiCtrl