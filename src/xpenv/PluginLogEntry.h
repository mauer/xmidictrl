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

#ifndef PLUGINLOGENTRY_H
#define PLUGINLOGENTRY_H

// Standard
#include <sstream>
#include <string>
#include <thread>

namespace XPEnv {

// Forward declarations
class PluginLogger;

// Enumerations
enum class LogLevel {
	Error = 0,
	Warn  = 1,
	Info  = 2,
	Debug = 3
};

// Structs
struct PluginLogData {
	LogLevel    level;
	time_t      time;
	std::string text;
};

class PluginLogEntry {
public:
	PluginLogEntry();

	static PluginLogEntry& Error(PluginLogEntry& logEntry);
	static PluginLogEntry& Warn(PluginLogEntry& logEntry);
	static PluginLogEntry& Info(PluginLogEntry& logEntry);
	static PluginLogEntry& Debug(PluginLogEntry& logEntry);
	static PluginLogEntry& Endl(PluginLogEntry& logEntry);

    PluginLogEntry& operator<<(int32_t i);
    PluginLogEntry& operator<<(int16_t i);
    PluginLogEntry& operator<<(unsigned int i);
	PluginLogEntry& operator<<(size_t s);
    PluginLogEntry& operator<<(double d);
    PluginLogEntry& operator<<(char c);
    PluginLogEntry& operator<<(std::string_view s);
	PluginLogEntry& operator<<(const std::thread::id& id);

	PluginLogEntry& operator<<(PluginLogEntry& (*f)(PluginLogEntry&));

private:
	LogLevel m_logLevel;

	std::ostringstream m_stream;
};

} // Namespace XPEnv

#endif // PLUGINLOGENTRY_H