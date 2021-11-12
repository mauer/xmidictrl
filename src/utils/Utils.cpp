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

// XMidiCtrl

#include "Utils.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the text of a message type
 */
std::string Utils::getMessageTypeText(MessageType messageType) {
    switch (messageType) {
        case MessageType::All:
            return "All";

        case MessageType::Error:
            return "Error";

        case MessageType::Warn:
            return "Warning";

        case MessageType::Info:
            return "Information";

        case MessageType::Debug:
            return "Debug";
    }

    return std::string("Debug");
}


/**
 * Return the text of a log level
 */
std::string Utils::getLogLevelText(LogLevel logLevel) {
    switch (logLevel) {
        case LogLevel::Error:
            return "Errors";

        case LogLevel::Warn:
            return "Warnings";

        case LogLevel::Info:
            return "Information";

        case LogLevel::Debug:
            return "Debug";
    }

    return std::string("Debug");
}


/**
 * Return the code of a log level
 */
std::string Utils::getLogLevelCode(LogLevel logLevel) {
    switch (logLevel) {
        case LogLevel::Error:
            return "E";

        case LogLevel::Warn:
            return "W";

        case LogLevel::Info:
            return "I";

        case LogLevel::Debug:
            return "D";
    }

    return std::string("D");
}


/**
 * Return the log level for a given code
 */
LogLevel Utils::getLogLevelFromCode(std::string_view code) {
    if (code == "E")
        return LogLevel::Error;
    else if (code == "W")
        return LogLevel::Warn;
    else if (code == "I")
        return LogLevel::Info;
    else
        return LogLevel::Debug;
}

} // Namespace XMidiCtrl
