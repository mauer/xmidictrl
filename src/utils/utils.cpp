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

#include "utils.h"

// Standard
#include <ctime>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the value of a string parameter
 */
std::string utils::read_string_parameter(toml::value &settings, std::string_view parameter, bool mandatory)
{
    std::string value;

    if (parameter.empty()) {
        LOG_ERROR << "Internal error (map::read_parameter --> parameter is empty" << LOG_END
        return {};
    }

    try {
        // read dataref
        if (settings.contains(parameter.data())) {
            value = settings[parameter.data()].as_string();
            LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << parameter << "' = '"
                      << value << "'" << LOG_END
        } else {
            if (mandatory) {
                LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str()
                          << LOG_END
                LOG_ERROR << " --> Parameter '" << parameter << "' not found" << LOG_END
            }
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return value;
}


/**
 * Read the value of an integer
 */
int utils::read_int_parameter(toml::value &settings, std::string_view parameter, bool mandatory)
{
    int value = -1;

    if (parameter.empty()) {
        LOG_ERROR << "Internal error (map::read_parameter --> parameter is empty" << LOG_END
        return value;
    }

    try {
        // read dataref
        if (settings.contains(parameter.data())) {
            value = settings[parameter.data()].as_integer();
            LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << parameter << "' = '"
                      << value << "'" << LOG_END
        } else {
            if (mandatory) {
                LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str()
                          << LOG_END
                LOG_ERROR << " --> Parameter '" << parameter << "' not found" << LOG_END
            }
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return value;
}


/**
 * Return the text of a text message type
 */
std::string utils::text_msg_type_as_text(text_msg_type type)
{
    switch (type) {
        case text_msg_type::all:
            return "All";

        case text_msg_type::error:
            return "Error";

        case text_msg_type::warn:
            return "Warning";

        case text_msg_type::info:
            return "Information";

        case text_msg_type::debug:
            return "Debug";
    }

    return {"Debug"};
}


/**
 * Return the text of a midi message type
 */
std::string utils::midi_msg_type_as_text(midi_type type)
{
    switch (type) {
        case midi_type::inbound:
            return "Inbound";

        case midi_type::outbound:
            return "Outbound";
    }

    return "Unknown";
}


/**
 * Return the text of a log level
 */
std::string utils::log_level_as_text(log_level level)
{
    switch (level) {
        case log_level::error:
            return "Errors";

        case log_level::warn:
            return "Warnings";

        case log_level::info:
            return "Information";

        case log_level::debug:
            return "Debug";

        default:
            return {"Debug"};
    }
}


/**
 * Return the code of a log level
 */
std::string utils::log_level_as_code(log_level level)
{
    switch (level) {
        case log_level::error:
            return "E";

        case log_level::warn:
            return "W";

        case log_level::info:
            return "I";

        case log_level::debug:
            return "D";

        default:
            return {"D"};
    }
}


/**
 * Return the log level for a given code
 */
log_level utils::log_level_from_code(std::string_view code)
{
    if (code == "E")
        return log_level::error;
    else if (code == "W")
        return log_level::warn;
    else if (code == "I")
        return log_level::info;
    else
        return log_level::debug;
}


/**
 * Convert a time point into a string
 */
std::string utils::time_to_string(time_point time)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&t);

    // format into a string
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}


/**
 * Return MIDI channel and control change as combined string
 */
std::string utils::ch_cc(const int ch, const int cc) {
    return std::to_string(ch) + "_" + std::to_string(cc);
}

} // Namespace xmidictrl
