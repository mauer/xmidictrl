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

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the value of a parameter
 */
std::string utils::read_config_parameter(toml::value &settings, std::string_view parameter, bool mandatory)
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
 * Return the text of a message type
 */
std::string utils::get_message_type_text(message_type type)
{
    switch (type) {
        case message_type::all:
            return "All";

        case message_type::error:
            return "Error";

        case message_type::warn:
            return "Warning";

        case message_type::info:
            return "Information";

        case message_type::debug:
            return "Debug";
    }

    return {"Debug"};
}


/**
 * Return the text of a log level
 */
std::string utils::get_log_level_Text(log_level level)
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
std::string utils::get_log_level_code(log_level level)
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
log_level utils::get_log_level_from_code(std::string_view code)
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

} // Namespace xmidictrl
