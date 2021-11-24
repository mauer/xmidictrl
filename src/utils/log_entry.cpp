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

#include "log_entry.h"

// Standard
#include <chrono>
#include <ctime>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Log a message regardless of the current log level
 */
log_entry &log_entry::all(log_entry &entry) {
    entry.set_message_type(message_type::all);
    return entry;
}


/**
 * Log an error message
 */
log_entry &log_entry::error(log_entry &entry) {
    entry.set_message_type(message_type::error);
    return entry;
}


/**
 * Log a warning message
 */
log_entry &log_entry::warn(log_entry &entry) {
    entry.set_message_type(message_type::warn);
    return entry;
}


/**
 * Log an info message
 */
log_entry &log_entry::info(log_entry &entry) {
    entry.set_message_type(message_type::info);
    return entry;
}


/**
 * Log a debug message
 */
log_entry &log_entry::debug(log_entry &entry) {
    entry.set_message_type(message_type::debug);
    return entry;
}


/**
 * Log the end of a message
 */
log_entry &log_entry::endLine(log_entry &entry) {
    std::shared_ptr<message> msg = std::make_shared<message>();

    // get current date time stamp
    time_t t = std::time(nullptr);
    struct tm *ltime = localtime(&t);

    // format into a string
    char datetime_str[32];
    std::strftime(&datetime_str[0], sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", ltime);

    msg->type = entry.get_message_type();
    msg->time = datetime_str;
    msg->text = entry.message_text();

    logger::instance().post_message(msg);

    // clear data and return
    entry.clear();
    return entry;
} 


/**
 * Log an int16 value
 */
log_entry &log_entry::operator<<(int16_t i) {
    m_stream << i;
    return *this;
}


/**
 * Log an int32 value
 */
log_entry &log_entry::operator<<(int32_t i) {
    m_stream << i;
    return *this;
}


/**
 * Log an unsigned int value
 */
log_entry &log_entry::operator<<(unsigned int i) {
    m_stream << i;
    return *this;
}


/**
 * Log a size_type value
 */
log_entry &log_entry::operator<<(size_t s) {
    m_stream << s;
    return *this;
}


/**
 * Log a double value
 */
log_entry &log_entry::operator<<(double d) {
    m_stream << d;
    return *this;
}


/**
 * Log a char value
 */
log_entry &log_entry::operator<<(char c) {
    m_stream << c;
    return *this;
}


/**
 * Log a string value
 */
log_entry &log_entry::operator<<(std::string_view s) {
    m_stream << s;
    return *this;
}


/**
 * Log a thread id
 */
log_entry &log_entry::operator<<(const std::thread::id& id) {
    m_stream << id;
    return *this;
}


/**
 * Invoke a function pointer to the stream
 */
log_entry &log_entry::operator<<(log_entry& (*f)(log_entry&)) {
    (*f)(*this);
    return *this;
}


/**
 * Clear the data
 */
void log_entry::clear() {
    m_type = message_type::debug;
    m_stream.clear();
}


/**
 * Set the message type
 */
void log_entry::set_message_type(message_type type) {
    m_type = type;
}


/**
 * Return the message type
 */
message_type log_entry::get_message_type() {
    return m_type;
}


/**
 * Return the message text
 */
std::string log_entry::message_text() {
    return m_stream.str();
}

} // Namespace xmidictrl