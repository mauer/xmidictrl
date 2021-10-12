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

#ifndef LOGGER_H_
#define LOGGER_H_

// Standard
#include <fstream>
#include <string_view>
#include <vector>

// XMidiCtrl
#include "LogEntry.h"

// Macros for logging
#define LOG_ERROR (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::Error)
#define LOG_WARN  (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::Info)
#define LOG_INFO  (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::Info)
#define LOG_DEBUG (XMidiCtrl::LogEntry() << XMidiCtrl::LogEntry::Debug)

#define LOG_END XMidiCtrl::LogEntry::Endl;

namespace XMidiCtrl {

class Logger {
public:
	Logger();
	~Logger();

	// Object cannot be copied, because of the stream
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

	static Logger& Instance();

	void initialise(std::string_view path, std::string_view pluginName);
	void postData(const PluginLogData& logData);

private:
	LogLevel m_logLevel;

	std::ofstream m_stream;

	std::vector<std::string> m_errors;
};

} // Namespace XMidiCtrl

#endif // LOGGER_H