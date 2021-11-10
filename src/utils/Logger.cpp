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

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Logger::Logger() {
    // initially we set the highest log level
    m_logLevel = LogLevel::Debug;
    m_messages = std::make_shared<MessageList>();
}


/**
 * Destructor
 */
Logger::~Logger() {
    if (m_stream.is_open())
        m_stream.close();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and return the logger instance
 */
Logger& Logger::Instance() {
    static Logger logger;
    return logger;
}


/**
 * Initialise the logger
 */
void Logger::initialise(std::string_view path) {
    if (!path.empty()) {
        std::string fileName = std::string(path) + XMIDICTRL_NAME + LOGFILE_SUFFIX;

        m_stream.open(fileName, std::ios_base::out | std::ios_base::trunc);
    }
}


/**
 * Set the active log loevel
 */
void Logger::setLogLevel(LogLevel logLevel) {
    m_logLevel = logLevel;
}


/**
 * Post a log entry
 */
void Logger::postMessage(Message::ptr message) {
    if (!checkLogLevel(message->type))
        return;

    // add message to internal list
    m_messages->addMessage(message);

    if (!m_stream.is_open())
        return;

    switch (message->type) {
        case MessageType::All:
            m_stream << message->time << "   " << "[ALL]" << "     " << message->text << std::endl;
            break;

        case MessageType::Error:
            m_stream << message->time << "   " << "[ERROR]" << "   " << message->text << std::endl;
            break;

        case MessageType::Warn:
            m_stream << message->time << "   " << "[WARN]" << "    " << message->text << std::endl;
            break;

        case MessageType::Info:
            m_stream << message->time << "   " << "[INFO]" << "    " << message->text << std::endl;
            break;

        case MessageType::Debug:
            m_stream << message->time << "   " << "[DEBUG]" << "   " << message->text << std::endl;
            break;
    }
}


/**
 * Return the internal message list
 */
MessageList::ptr Logger::messages() {
    return m_messages;
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check the current log level
 */
bool Logger::checkLogLevel(MessageType messageType) {
    switch (messageType) {
        case MessageType::All:
            return true;

        case MessageType::Error:
            return true;

        case MessageType::Warn:
            if (m_logLevel != LogLevel::Error)
                return true;
            break;

        case MessageType::Info:
            if (m_logLevel == LogLevel::Info || m_logLevel == LogLevel::Debug)
                return true;
            break;

        case MessageType::Debug:
            if (m_logLevel == LogLevel::Debug)
                return true;
            break;
    }

    return false;
}


} // Namespace XMidiCtrl