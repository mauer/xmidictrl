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

#ifndef COMMANDS_H
#define COMMANDS_H

// Standard
#include <map>
#include <memory>
#include <string_view>

// X-Plane SDK
#include "XPLMUtilities.h"

// XMidiCtrl
#include "text_logger.h"

namespace xmidictrl {

class commands {
public:
	commands() = default;
    ~commands() = default;

    void begin(text_logger &in_log, std::string_view in_cmd);
    void end(text_logger &in_log, std::string_view in_cmd);
    
    void execute(text_logger &in_log, std::string_view in_cmd);

private:
    XPLMCommandRef find_command_ref(text_logger &in_log, std::string_view in_cmd);

    std::map<std::string, XPLMCommandRef> m_command_cache {};
};

} // Namespace xmidictrl

#endif // COMMANDS_H