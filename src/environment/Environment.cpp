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
#include "Environment.h"
#include "Logger.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Environment::Environment() {
    // read the general settings first
    m_settings = std::make_shared<Settings>();

    // internal message list
    m_messages = std::make_shared<MessageList>(m_settings);

    // access to X-Plane commands
    m_commands = std::make_shared<Commands>();

    // access to X-Plane datarefs
    m_data = std::make_shared<Data>();
}


/**
 * Destructor
 */
Environment::~Environment() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Show an info message on the screen
 */
void Environment::raiseInfoMessage(std::string_view text) {
    LOG_INFO << text << LOG_END
    m_messages->addMessage(MessageType::Info, text);
}


/**
 * Show an error message on the screen
 */
void Environment::raiseErrorMessage(std::string_view text) {
    LOG_ERROR << text << LOG_END
    m_messages->addMessage(MessageType::Error, text);
}


/**
 * Return the message list
 */
MessageList::ptr Environment::messages() {
    return m_messages;
}


/**
 * Return the commands object
 */
Commands::ptr Environment::commands() {
    return m_commands;
}


/**
 * Return the dataref object
 */
Data::ptr Environment::data() {
    return m_data;
}

} // Namespace XMidiCtrl