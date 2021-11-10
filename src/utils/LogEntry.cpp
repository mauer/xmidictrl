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
#include "LogEntry.h"
#include "Logger.h"
#include "Message.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
LogEntry::LogEntry() {
    // Default setting
    m_messageType = MessageType::Debug;
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Log a message regardless of the current log level
 */
LogEntry& LogEntry::all(LogEntry& logEntry) {
    logEntry.setMessageType(MessageType::All);
    return logEntry;
}


/**
 * Log an error message
 */
LogEntry& LogEntry::error(LogEntry& logEntry) {
    logEntry.setMessageType(MessageType::Error);
    return logEntry;
}


/**
 * Log a warning message
 */
LogEntry& LogEntry::warn(LogEntry& logEntry) {
    logEntry.setMessageType(MessageType::Warn);
    return logEntry;
}


/**
 * Log an info message
 */
LogEntry& LogEntry::info(LogEntry& logEntry) {
    logEntry.setMessageType(MessageType::Info);
    return logEntry;
}


/**
 * Log a debug message
 */
LogEntry& LogEntry::debug(LogEntry& logEntry) {
    logEntry.setMessageType(MessageType::Debug);
    return logEntry;
}


/**
 * Log the end of a message
 */
LogEntry& LogEntry::endLine(LogEntry& logEntry) {
    Message::ptr message = std::make_shared<Message>();

    // get current date time stamp
    time_t t = std::time(nullptr);
    struct tm* lTime = localtime(&t);

    // format into a string
    char dateTimeStr[32];
    std::strftime(&dateTimeStr[0], sizeof(dateTimeStr), "%Y-%m-%d %H:%M:%S", lTime);

    message->type = logEntry.messageType();
    message->time = dateTimeStr;
    message->text = logEntry.messageText();

    Logger::Instance().postMessage(message);

    // clear data and return
    logEntry.clear();
    return logEntry;
} 


/**
 * Log an int16 value
 */
LogEntry& LogEntry::operator<<(int16_t i) {
    m_stream << i;
    return *this;
}


/**
 * Log an int32 value
 */
LogEntry& LogEntry::operator<<(int32_t i) {
    m_stream << i;
    return *this;
}


/**
 * Log an unsigned int value
 */
LogEntry& LogEntry::operator<<(unsigned int i) {
    m_stream << i;
    return *this;
}


/**
 * Log a size_type value
 */
LogEntry& LogEntry::operator<<(size_t s) {
    m_stream << s;
    return *this;
}


/**
 * Log a double value
 */
LogEntry& LogEntry::operator<<(double d) {
    m_stream << d;
    return *this;
}


/**
 * Log a char value
 */
LogEntry& LogEntry::operator<<(char c) {
    m_stream << c;
    return *this;
}


/**
 * Log a string value
 */
LogEntry& LogEntry::operator<<(std::string_view s) {
    m_stream << s;
    return *this;
}


/**
 * Log a thread id
 */
LogEntry& LogEntry::operator<<(const std::thread::id& id) {
    m_stream << id;
    return *this;
}


/**
 * Invoke a function pointer to the stream
 */
LogEntry& LogEntry::operator<<(LogEntry& (*f)(LogEntry&)) {
    (*f)(*this);
    return *this;
}


/**
 * Clear the data
 */
void LogEntry::clear() {
    m_messageType = MessageType::Debug;
    m_stream.clear();
}


/**
 * Set the message type
 */
void LogEntry::setMessageType(MessageType messageType) {
    m_messageType = messageType;
}


/**
 * Return the message type
 */
MessageType LogEntry::messageType() {
    return m_messageType;
}


/**
 * Return the message text
 */
std::string LogEntry::messageText() {
    return m_stream.str();
}

} // Namespace XMidiCtrl