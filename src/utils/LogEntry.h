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

#ifndef LOGENTRY_H
#define LOGENTRY_H

// Standard
#include <memory>
#include <sstream>
#include <string>
#include <thread>

// XMidiCtrl
#include "Types.h"

namespace XMidiCtrl {

// Forward declarations
class Logger;


class LogEntry {
public:
	LogEntry();

    static LogEntry& all(LogEntry& logEntry);
	static LogEntry& error(LogEntry& logEntry);
	static LogEntry& warn(LogEntry& logEntry);
	static LogEntry& info(LogEntry& logEntry);
	static LogEntry& debug(LogEntry& logEntry);
	static LogEntry& endLine(LogEntry& logEntry);

    LogEntry& operator<<(int32_t i);
    LogEntry& operator<<(int16_t i);
    LogEntry& operator<<(unsigned int i);
	LogEntry& operator<<(size_t s);
    LogEntry& operator<<(double d);
    LogEntry& operator<<(char c);
    LogEntry& operator<<(std::string_view s);
	LogEntry& operator<<(const std::thread::id& id);

	LogEntry& operator<<(LogEntry& (*f)(LogEntry&));

    void clear();

    void setMessageType(MessageType messageType);
    MessageType messageType();

    std::string messageText();

private:
	MessageType m_messageType;

	std::ostringstream m_stream;
};

} // Namespace XMidiCtrl

#endif // LOGENTRY_H