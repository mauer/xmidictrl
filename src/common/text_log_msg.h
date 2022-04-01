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

#ifndef TEXT_LOG_MSG_H
#define TEXT_LOG_MSG_H

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

// Structs
struct text_log_msg {
    std::string time;
    log_level level;
    std::string text;

    // Return the message type as text
    [[nodiscard]] std::string get_log_level_text() const
    {
        switch (level) {
            case log_level::error:
                return "Error";

            case log_level::warn:
                return "Warning";

            case log_level::info:
                return "Information";

            case log_level::debug:
                return "Debug";
        }

        return "<unknown>";
    }
};

} // Namespace xmidictrl

#endif // TEXT_LOG_MSG_H