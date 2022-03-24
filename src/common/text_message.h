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

#ifndef TEXT_MESSAGE_H
#define TEXT_MESSAGE_H

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

// Structs
struct text_message {
    std::string time;
    text_msg_type type;
    std::string text;

    // Return the message type as text
    [[nodiscard]] std::string get_type_text() const
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

            default:
                return "<unknown>";
        }
    }
};

} // Namespace xmidictrl

#endif // TEXT_MESSAGE_H