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
#include "MappingSlider.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MappingSlider::MappingSlider(int cc)
        : Mapping(cc) {}




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
 * Set the top command
 */
void MappingSlider::setCommandTop(std::string_view commandTop) {
    m_commandTop = commandTop;
}


/**
 * Return the top command
 */
std::string_view MappingSlider::commandTop() const {
    return m_commandTop;
}


/**
 * Set the bottom command
 */
void MappingSlider::setCommandBottom(std::string_view commandBottom) {
    m_commandBottom = commandBottom;
}


/**
 * Return the bottom command
 */
std::string_view MappingSlider::commandBottom() const {
    return m_commandBottom;
}


/**
 * Check the mapping
 */
bool MappingSlider::check() {
    if (!Mapping::check())
        return false;

    if (m_commandTop.empty() && m_commandBottom.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void MappingSlider::execute(Environment::ptr environment, MidiEvent::ptr midiEvent) {
    LOG_DEBUG << "MappingSlider::execute" << LOG_END

    if (midiEvent->velocity() <= 10)
        environment->commands()->execute(m_commandBottom);
    else if (midiEvent->velocity() >= 117)
        environment->commands()->execute(m_commandTop);
}

} // Namespace XMidiCtrl