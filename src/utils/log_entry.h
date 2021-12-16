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

#ifndef LOG_ENTRY_H
#define LOG_ENTRY_H

// Standard
#include <memory>
#include <sstream>
#include <string>
#include <thread>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

// Forward declarations
class logger;

class log_entry {
public:
    log_entry() = default;
    ~log_entry() = default;

    static log_entry &all(log_entry &entry);
    static log_entry &error(log_entry &entry);
    static log_entry &warn(log_entry &entry);
    static log_entry &info(log_entry &entry);
    static log_entry &debug(log_entry &entry);
    static log_entry &endLine(log_entry &entry);

    log_entry &operator<<(int32_t i);
    log_entry &operator<<(int16_t i);
    log_entry &operator<<(unsigned int i);
    log_entry &operator<<(size_t s);
    log_entry &operator<<(double d);
    log_entry &operator<<(char c);
    log_entry &operator<<(std::string_view s);
    log_entry &operator<<(const std::thread::id &id);

    log_entry &operator<<(log_entry &(*f)(log_entry &));

    void clear();

    void set_message_type(text_msg_type type);
    text_msg_type get_message_type();

    std::string message_text();

private:
    text_msg_type m_type {text_msg_type::debug};

    std::ostringstream m_stream;
};

} // Namespace xmidictrl

#endif // LOG_ENTRY_H