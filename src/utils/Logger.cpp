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
#include <chrono>
#include <ctime>

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
    m_logLevel = LogLevel::Error;
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
void Logger::initialise(std::string_view path, std::string_view pluginName) {
    if (!path.empty() && !pluginName.empty()) {
        std::string fileName = std::string(path) + std::string(pluginName) + "_log.txt";

        m_stream.open(fileName, std::ios_base::out | std::ios_base::trunc);
    }
}


/**
 * Post a log entry
 */
void Logger::postData(const PluginLogData& logData) {
    if (!m_stream.is_open())
        return;

    // format datetime stamp
    char dateTimeStr[32];
    time_t t = time(nullptr);
    struct tm *tm = localtime(&t);

    std::strftime(&dateTimeStr[0], sizeof(dateTimeStr), "%Y-%m-%d %H:%M:%S", tm);

    switch (logData.level) {
        case LogLevel::Error:
            m_stream << dateTimeStr << "   " << "[ERROR]" << "   " << logData.text << std::endl;
            m_messages->addMessage(MessageType::Error, std::string(dateTimeStr) + "   " + logData.text);
            break;

        case LogLevel::Warn:
            m_stream << dateTimeStr << "   " << "[WARM]" << "    " << logData.text << std::endl;
            m_messages->addMessage(MessageType::Warn, std::string(dateTimeStr) + "   " + logData.text);
            break;

        case LogLevel::Info:
            m_stream << dateTimeStr << "   " << "[INFO]" << "    " << logData.text << std::endl;
            m_messages->addMessage(MessageType::Info, std::string(dateTimeStr) + "   " + logData.text);
            break;

        case LogLevel::Debug:
            m_stream << dateTimeStr << "   " << "[DEBUG]" << "   " << logData.text << std::endl;
            break;
    }
}


/**
 * Return the internal message list
 */
MessageList::ptr Logger::messages() {
    return m_messages;
}

} // Namespace XMidiCtrl