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
#include "MappingCommand.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MappingCommand::MappingCommand(int cc)
        : Mapping(cc) {}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
MappingType MappingCommand::type() {
    return MappingType::Command;
};


/**
 * Set the command
 */
void MappingCommand::setCommand(std::string_view command) {
    m_command = command;
    LOG_DEBUG << "MappingCommand::setCommand :: command = '" << command.data() << "'" << LOG_END
}


/**
 * Return the command
 */
std::string_view MappingCommand::command() const {
    return m_command;
}


/**
 * Check the mapping
 */
bool MappingCommand::check() {
    if (!Mapping::check())
        return false;

    if (m_command.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void MappingCommand::execute(Environment::ptr environment, MidiEvent::ptr midiEvent) {
    LOG_DEBUG << "MappingCommand :: Execute command '" << m_command.data() << "'" << LOG_END

    switch (midiEvent->velocity()) {
        case 127:
            LOG_DEBUG << "Execute begin command '" << m_command.data() << "'" << LOG_END
            environment->commands()->begin(m_command);
            break;

        case 0:
            LOG_DEBUG << "Execute end command' " << m_command.data() << "'" << LOG_END
            environment->commands()->end(m_command);
            break;

        default:
            LOG_ERROR << "Invalid Midi status '" << midiEvent->status() << "'" << LOG_END
    }

    //environment->commands()->execute(m_command);
}

} // Namespace XMidiCtrl