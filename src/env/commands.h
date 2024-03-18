//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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
#include <string_view>

// XMidiCtrl
#include "text_logger.h"

namespace xmidictrl {

class commands {
public:
	commands() = default;
    virtual ~commands() = default;

    virtual void begin(text_logger &in_log, std::string_view in_cmd) = 0;
    virtual void end(text_logger &in_log, std::string_view in_cmd) = 0;
    
    virtual void execute(text_logger &in_log, std::string_view in_cmd) = 0;
};

} // Namespace xmidictrl

#endif // COMMANDS_H