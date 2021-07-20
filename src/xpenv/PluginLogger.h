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

#ifndef PLUGINLOGGER_H_
#define PLUGINLOGGER_H_

// Standard
#include <fstream>
#include <string_view>

// X-Plane Environment
#include "PluginLogEntry.h"

// Macros for logging
#define LOG_ERROR (XPEnv::PluginLogEntry() << XPEnv::PluginLogEntry::Error)
#define LOG_WARN (XPEnv::PluginLogEntry() << XPEnv::PluginLogEntry::Info)
#define LOG_INFO (XPEnv::PluginLogEntry() << XPEnv::PluginLogEntry::Info)
#define LOG_DEBUG (XPEnv::PluginLogEntry() << XPEnv::PluginLogEntry::Debug)

#define LOG_END XPEnv::PluginLogEntry::endl;


namespace XPEnv {

class PluginLogger {
public:
	PluginLogger();
	~PluginLogger();

	// Object cannot be copied, because of the stream
    PluginLogger(const PluginLogger&) = delete;
    PluginLogger& operator=(const PluginLogger&) = delete;

	static PluginLogger& Instance();

	void initialise(std::string_view path, std::string_view pluginName);
	void postData(const PluginLogData logData);

private:
	LogLevel m_logLevel;

	std::ofstream m_stream;
};

} // XPEnv

#endif // PLUGINLOGGER_H_