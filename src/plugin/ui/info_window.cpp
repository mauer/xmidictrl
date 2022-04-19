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

#include "info_window.h"

// Standard
#include <string>
#include <utility>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
info_window::info_window(text_logger &in_log,
                         xplane &in_xp,
                         settings &in_settings,
                         std::map<std::string, std::shared_ptr<info_msg>> &in_msg)
    : imgui_window(in_log, in_xp, 850, 80, window_position::bottom_left, 50, 50, true),
      m_settings(in_settings),
      m_messages(in_msg)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Show the info window
 */
void info_window::show()
{
    imgui_window::show();

    set_window_position(m_settings.info_position(),
                        850,
                        (int) (m_messages.size() * ROW_HEIGHT) + 55,
                        m_settings.info_offset_x(),
                        m_settings.info_offset_y());
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void info_window::create_widgets()
{
    ImGui::NewLine();

    for (const std::pair<const std::basic_string<char>, std::shared_ptr<info_msg>> &msg: m_messages)
        ImGui::TextUnformatted(msg.second->text.c_str());
}

} // Namespace xmidictrl
