//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
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
    entry.set_message_type(text_msg_type::all);
    return entry;
}


/**
 * Log an error message
 */
log_entry &log_entry::error(log_entry &entry) {
    entry.set_message_type(text_msg_type::error);
    return entry;
}


/**
 * Log a warning message
 */
log_entry &log_entry::warn(log_entry &entry) {
    entry.set_message_type(text_msg_type::warn);
    return entry;
}


/**
 * Log an info message
 */
log_entry &log_entry::info(log_entry &entry) {
    entry.set_message_type(text_msg_type::info);
    return entry;
}


/**
 * Log a debug message
 */
log_entry &log_entry::debug(log_entry &entry) {
    entry.set_message_type(text_msg_type::debug);
    return entry;
}


/**
 * Log the end of a message
 */
log_entry &log_entry::endLine(log_entry &entry) {
    std::shared_ptr<text_message> msg = std::make_shared<text_message>();

    // get current date time stamp
    time_t t = std::time(nullptr);
    struct tm *ltime = localtime(&t);

    // format into a string
    char datetime_str[32];
    std::strftime(&datetime_str[0], sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", ltime);

    msg->type = entry.get_message_type();
    msg->time = datetime_str;
    msg->text = entry.message_text();

    logger::instance().post_text_message(msg);

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
    m_type = text_msg_type::debug;
    m_stream.clear();
}


/**
 * Set the message type
 */
void log_entry::set_message_type(text_msg_type type) {
    m_type = type;
}


/**
 * Return the message type
 */
text_msg_type log_entry::get_message_type() {
    return m_type;
}


/**
 * Return the message text
 */
std::string log_entry::message_text() {
    return m_stream.str();
}

} // Namespace xmidictrl