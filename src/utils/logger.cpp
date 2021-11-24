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

#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
logger::logger()
{
    m_messages = std::make_shared<message_list>();
}


/**
 * Destructor
 */
logger::~logger()
{
    if (m_stream.is_open())
        m_stream.close();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and return the logger instance
 */
logger &logger::instance()
{
    static logger log;
    return log;
}


/**
 * Initialise the logger
 */
void logger::init(std::string_view path)
{
    if (!path.empty()) {
        std::string filename = std::string(path) + XMIDICTRL_NAME + LOGFILE_SUFFIX;

        m_stream.open(filename, std::ios_base::out | std::ios_base::trunc);
    }
}


/**
 * Set the active log level
 */
void logger::set_log_level(log_level logLevel)
{
    m_log_level = logLevel;
}


/**
 * Return the current log level
 */
log_level logger::get_log_Level() const
{
    return m_log_level;
}


/**
 * Post a log entry
 */
void logger::post_message(std::shared_ptr<message> msg)
{
    if (!check_log_level(msg->type))
        return;

    // add message to internal list
    m_messages->add_message(msg);

    if (!m_stream.is_open())
        return;

    switch (msg->type) {
        case message_type::all:
            m_stream << msg->time << "   " << "[ALL]" << "     " << msg->text << std::endl;
            break;

        case message_type::error:
            m_stream << msg->time << "   " << "[ERROR]" << "   " << msg->text << std::endl;
            break;

        case message_type::warn:
            m_stream << msg->time << "   " << "[WARN]" << "    " << msg->text << std::endl;
            break;

        case message_type::info:
            m_stream << msg->time << "   " << "[INFO]" << "    " << msg->text << std::endl;
            break;

        case message_type::debug:
            m_stream << msg->time << "   " << "[DEBUG]" << "   " << msg->text << std::endl;
            break;
    }
}


/**
 * Return the internal message list
 */
message_list::ptr logger::messages()
{
    return m_messages;
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check the current log level
 */
bool logger::check_log_level(message_type type)
{
    switch (type) {
        case message_type::all:
        case message_type::error:
            return true;

        case message_type::warn:
            if (m_log_level != log_level::error)
                return true;
            break;

        case message_type::info:
            if (m_log_level == log_level::info || m_log_level == log_level::debug)
                return true;
            break;

        case message_type::debug:
            if (m_log_level == log_level::debug)
                return true;
            break;
    }

    return false;
}

} // Namespace xmidictrl