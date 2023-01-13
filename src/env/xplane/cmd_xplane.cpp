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

#include "cmd_xplane.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Begin a X-Plane command
 */
void cmd_xplane::begin(text_logger &in_log, std::string_view in_cmd)
{
    XPLMCommandRef cmd_ref = find_command_ref(in_log, in_cmd);

    if (cmd_ref != nullptr)
        XPLMCommandBegin(cmd_ref);
}


/**
 * End a X-Plane command
 */
void cmd_xplane::end(text_logger &in_log, std::string_view in_cmd)
{
    XPLMCommandRef cmd_ref = find_command_ref(in_log, in_cmd);

    if (cmd_ref != nullptr)
        XPLMCommandEnd(cmd_ref);
}


/**
 * Execute a X-Plane command
 */
void cmd_xplane::execute(text_logger &in_log, std::string_view in_cmd)
{
    XPLMCommandRef cmd_ref = find_command_ref(in_log, in_cmd);

    if (cmd_ref != nullptr)
        XPLMCommandOnce(cmd_ref);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the command ref for a command string
 */
XPLMCommandRef cmd_xplane::find_command_ref(text_logger &in_log, std::string_view in_cmd)
{
    XPLMCommandRef cmd_ref = nullptr;

    // check the cache first
    try {
        cmd_ref = m_command_cache.at(in_cmd.data());
    } catch (std::out_of_range const &) {
        cmd_ref = XPLMFindCommand(in_cmd.data());
        m_command_cache.emplace(in_cmd, cmd_ref);
    }

    if (cmd_ref == nullptr)
        in_log.error("Command '" + std::string(in_cmd) + "' not found");

    return cmd_ref;
}

} // Namespace xmidictrl