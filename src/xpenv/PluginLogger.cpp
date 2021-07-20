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
#include <time.h>

// X-Plane Environment
#include "PluginLogger.h"


namespace XPEnv {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
PluginLogger::PluginLogger() {
    m_logLevel = LogLevel::Error;
}


/**
 * Destructor
 */
PluginLogger::~PluginLogger() {
    if (m_stream.is_open())
        m_stream.close();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and return the logger instance
 */
PluginLogger& PluginLogger::Instance() {
    static PluginLogger logger;

    return logger;
}


/**
 * Initialise the logger
 */
void PluginLogger::initialise(std::string_view path, std::string_view pluginName) {
    if (!path.empty() && !pluginName.empty()) {
        std::string fileName = std::string(path) + std::string(pluginName) + "_log.txt";

        m_stream.open(fileName, std::ios_base::out | std::ios_base::trunc);
    }
}


/**
 * Post a log entry
 */
void PluginLogger::postData(const PluginLogData logData) {
    if (!m_stream.is_open())
        return;

    // format datetime stamp
    std::string dateTimeStr(19, '\0');
    struct tm time;
    localtime_s(&time, &logData.time);
    std::strftime(&dateTimeStr[0], dateTimeStr.size(), "%Y-%m-%d %H:%M:%S", &time);
  
    switch (logData.level) {
        case LogLevel::Error:
            m_stream << dateTimeStr << "   " << "[ERROR]" << "   " << logData.text << std::endl;
            break;

        case LogLevel::Warn:
            m_stream << dateTimeStr << "   " << "[WARM]" << "    " << logData.text << std::endl;
            break;

        case LogLevel::Info:
            m_stream << dateTimeStr << "   " << "[INFO]" << "    " << logData.text << std::endl;
            break;

        case LogLevel::Debug:
            m_stream << dateTimeStr << "   " << "[DEBUG]" << "   " << logData.text << std::endl;
            break;
    }
}

} // XPEnv