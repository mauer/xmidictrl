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
#include "MappingPushAndPull.h"

#include <utility>

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MappingPushAndPull::MappingPushAndPull(Environment::ptr environment, int controlChange)
        : Mapping(std::move(environment), controlChange) {
    m_commandType = CommandType::Push;
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
MappingType MappingPushAndPull::type() {
    return MappingType::PushAndPull;
};


/**
 * Set the push command
 */
void MappingPushAndPull::setCommandPush(std::string_view commandPush) {
    m_commandPush = commandPush;
}


/**
 * Return the push command
 */
std::string_view MappingPushAndPull::commandPush() const {
    return m_commandPush;
}


/**
 * Set the pull command
 */
void MappingPushAndPull::setCommandPull(std::string_view commandPull) {
    m_commandPull = commandPull;
}


/**
 * Return the pull command
 */
std::string_view MappingPushAndPull::commandPull() const {
    return m_commandPull;
}


/**
 * Set the command type
 */
void MappingPushAndPull::setCommandType(const CommandType commandType) {
    m_commandType = commandType;
}


/**
 * Check the mapping
 */
bool MappingPushAndPull::check() {
    if (!Mapping::check())
        return false;

    if (m_commandPush.empty() && m_commandPull.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void MappingPushAndPull::execute(MidiEvent::ptr midiEvent) {
    switch (m_commandType) {
        case CommandType::Push:
            m_environment->commands()->execute(m_commandPush);
            break;

        case CommandType::Pull:
            m_environment->commands()->execute(m_commandPull);
            break;
    }

}

} // Namespace XMidiCtrl