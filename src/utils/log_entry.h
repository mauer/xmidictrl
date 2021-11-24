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

#ifndef _LOG_ENTRY_H_
#define _LOG_ENTRY_H_

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

    void set_message_type(message_type type);
    message_type get_message_type();

    std::string message_text();

private:
    message_type m_type {message_type::debug};

    std::ostringstream m_stream;
};

} // Namespace xmidictrl

#endif // _LOG_ENTRY_H_