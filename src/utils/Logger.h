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

#ifndef LOGGER_H
#define LOGGER_H

// Standard
#include <fstream>
#include <memory>
#include <string_view>

// XMidiCtrl
#include "LogEntry.h"
#include "MessageList.h"

// Macros for logging
#define LOG_ALL   (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::all)
#define LOG_ERROR (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::error)
#define LOG_WARN  (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::warn)
#define LOG_INFO  (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::info)
#define LOG_DEBUG (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::debug)

#define LOG_END XMidiCtrl::LogEntry::endLine;

namespace XMidiCtrl {

class Logger {
public:
	Logger();
	~Logger();

	// Object cannot be copied, because of the stream
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

	static Logger& Instance();

	void initialise(std::string_view path);

    void setLogLevel(LogLevel logLevel);
    const LogLevel logLevel();

	void postMessage(Message::ptr message);

    MessageList::ptr messages();

private:
    bool checkLogLevel(MessageType messageType);

	LogLevel m_logLevel;

	std::ofstream m_stream;

	MessageList::ptr m_messages;
};

} // Namespace XMidiCtrl

#endif // LOGGER_H