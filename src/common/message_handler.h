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

#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

// Standard
#include <cstdarg>
#include <memory>
#include <string>
#include <string_view>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

class message_handler {
public:
    explicit message_handler(std::shared_ptr<message_handler> parent = nullptr);
    ~message_handler() = default;

    void clear();

    bool has_errors();

    void debug(std::string_view text, ...);
    void info(std::string_view text, ...);
    void warn(std::string_view text, ...);
    void error(std::string_view text, ...);

private:
    void create_message(text_msg_type type, std::string_view text, std::va_list args);

    std::shared_ptr<message_handler> m_parent;
};

} // Namespace xmidictrl

#endif // MESSAGE_HANDLER_H
