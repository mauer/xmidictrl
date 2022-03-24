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

#include "message_handler.h"

// fmtlib
#include <fmt/core.h>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
message_handler::message_handler(std::shared_ptr<message_handler> parent)
    : m_parent(std::move(parent))
{
}


//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Clear all messages
 */
void message_handler::clear()
{

}


/**
 * Return of error messages occurred
 */
bool message_handler::has_errors()
{
    return true;
}


void message_handler::debug(std::string_view text, ...)
{
    std::va_list args;
    va_start(args, text);

    create_message(text_msg_type::debug, text, args);

    va_end(args);
}


void message_handler::info(std::string_view text, ...)
{
    va_list args;
    va_start(args, text);

    create_message(text_msg_type::info, text, args);

    va_end(args);
}


void message_handler::warn(std::string_view text, ...)
{
    va_list args;
    va_start(args, text);

    create_message(text_msg_type::warn, text, args);

    va_end(args);
}


void message_handler::error(std::string_view text, ...)
{
    va_list args;
    va_start(args, text);

    create_message(text_msg_type::error, text, args);

    va_end(args);
}


void message_handler::create_message(const text_msg_type type, std::string_view text, std::va_list args)
{
    char msg_text[8129];
    vsnprintf(msg_text, sizeof(msg_text), text.data(), args);

    switch (type) {
        case text_msg_type::debug:
            LOG_DEBUG << msg_text << LOG_END
            break;

        case text_msg_type::info:
            LOG_INFO << msg_text << LOG_END
            break;

        case text_msg_type::warn:
            LOG_WARN << msg_text << LOG_END
            break;

        case text_msg_type::error:
            LOG_ERROR << msg_text << LOG_END
            break;

        case text_msg_type::all:
            LOG_ALL << msg_text << LOG_END
            break;
    }
}

} // Namespace xmidictrl
