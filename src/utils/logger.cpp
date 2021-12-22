//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#include "logger.h"

// XMidiCtrl
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
logger::~logger()
{
    m_text_messages.clear();
    m_midi_messages.clear();

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
void logger::init(std::string_view a_path, std::shared_ptr<settings> a_settings)
{
    if (!a_path.empty()) {
        std::string filename = std::string(a_path) + XMIDICTRL_NAME + LOGFILE_SUFFIX;

        m_stream.open(filename, std::ios_base::out | std::ios_base::trunc);
    }

    m_settings = std::move(a_settings);
}


/**
 * Return the current log level
 */
log_level logger::logging_Level() const
{
    return m_settings->logging_level();
}


/**
 * Return if midi logging is enabled
 */
bool logger::log_midi() const
{
    return m_settings->log_midi();
}


/**
 * Post a text message
 */
void logger::post_text_message(const std::shared_ptr<text_message> &msg)
{
    if (!check_log_level(msg->type))
        return;

    // add message to internal list
    std::shared_ptr<text_log_msg> log_msg = std::make_shared<text_log_msg>();
    log_msg->time = msg->time;
    log_msg->type = utils::text_msg_type_as_text(msg->type);
    log_msg->text = msg->text;

    m_text_messages.push_back(log_msg);

    // write message to log file
    if (!m_stream.is_open())
        return;

    switch (msg->type) {
        case text_msg_type::all:
            m_stream << msg->time << "   " << "[ALL]" << "     " << msg->text << std::endl;
            break;

        case text_msg_type::error:
            m_stream << msg->time << "   " << "[ERROR]" << "   " << msg->text << std::endl;
            break;

        case text_msg_type::warn:
            m_stream << msg->time << "   " << "[WARN]" << "    " << msg->text << std::endl;
            break;

        case text_msg_type::info:
            m_stream << msg->time << "   " << "[INFO]" << "    " << msg->text << std::endl;
            break;

        case text_msg_type::debug:
            m_stream << msg->time << "   " << "[DEBUG]" << "   " << msg->text << std::endl;
            break;
    }
}


/**
 * Post a midi message
 */
void logger::post_midi_message(const std::shared_ptr<midi_message> &msg)
{
    if (m_settings->log_midi()) {
        std::shared_ptr<midi_log_msg> log_msg = std::make_shared<midi_log_msg>();

        log_msg->time = utils::time_to_string(msg->time);
        log_msg->type = utils::midi_msg_type_as_text(msg->type);

        log_msg->port = msg->port;

        log_msg->status = msg->status;
        log_msg->data = msg->data;
        log_msg->velocity = msg->velocity;

        m_midi_messages.push_back(log_msg);
    }
}


/**
 * Clear all text messages
 */
void logger::clear_text_messages()
{
    m_text_messages.clear();
}


/**
 * Clear all midi messages
 */
void logger::clear_midi_messages()
{
    m_midi_messages.clear();
}


/**
 * Return the number of text message
 */
unsigned int logger::count_text_messages()
{
    return m_text_messages.size();
}


/**
 * Return the number of midi messages
 */
unsigned int logger::count_midi_messages()
{
    return m_midi_messages.size();
}


/**
 * Return a text message
 */
std::shared_ptr<text_log_msg> logger::get_text_message(int idx)
{
    try {
        return m_text_messages.at(idx);
    } catch (std::out_of_range &) {
        return nullptr;
    }
}


/**
 * Return a midi message
 */
std::shared_ptr<midi_log_msg> logger::get_midi_message(int idx)
{
    try {
        return m_midi_messages.at(idx);
    } catch (std::out_of_range &) {
        return nullptr;
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check the current log level
 */
bool logger::check_log_level(text_msg_type type)
{
    log_level level = log_level::debug;

    if (m_settings != nullptr)
        level = m_settings->logging_level();

    switch (type) {
        case text_msg_type::all:
        case text_msg_type::error:
            return true;

        case text_msg_type::warn:
            if (level != log_level::error)
                return true;
            break;

        case text_msg_type::info:
            if (level == log_level::info || level == log_level::debug)
                return true;
            break;

        case text_msg_type::debug:
            if (level == log_level::debug)
                return true;
            break;
    }

    return false;
}

} // Namespace xmidictrl