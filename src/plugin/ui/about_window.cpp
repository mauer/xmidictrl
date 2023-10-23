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

#include "about_window.h"

// Standard
#include <string>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
about_window::about_window(text_logger& in_log, environment& in_env)
    : imgui_window(in_log, in_env, 850, 460)
{
    set_title(std::string(XMIDICTRL_NAME) + " - About");
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void about_window::create_widgets()
{
    ImGui::NewLine();
    ImGui::TextColored(title_color(), "%s", XMIDICTRL_NAME " - " XMIDICTRL_DESCRIPTION);
    ImGui::NewLine();
    ImGui::TextColored(title_color(), "%s", "Version " XMIDICTRL_VERSION_STR);
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::TextUnformatted("Copyright (c) 2021-2023 by Marco Auer");
    ImGui::NewLine();
    ImGui::TextUnformatted(XMIDICTRL_NAME " has been released under the GNU Affero General Public License");
    ImGui::TextUnformatted("in hope that it will be useful, but without any warranty.");
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::TextUnformatted(XMIDICTRL_NAME " uses the following libraries:");
    ImGui::Bullet();
    ImGui::TextUnformatted("RtMidi from Gary P. Scavone (https://github.com/thestk/rtmidi)");
    ImGui::Bullet();
    ImGui::TextUnformatted("TOML for Modern C++ from Toru Niina (https:://github.com/ToruNiina/toml11)");
    ImGui::Bullet();
    ImGui::TextUnformatted("Dear ImGui from Omar Cornut (https:://github.com/ocornut/imgui)");
    ImGui::Bullet();
    ImGui::TextUnformatted("Font Awesome from Fonticons Inc. (https://fontawesome.com)");
    ImGui::Bullet();
    ImGui::TextUnformatted(
        "Dear ImGui X-Plane Integration from Christopher Collins (https://github.com/xsquawkbox/xsb_public)");
    ImGui::Bullet();
    ImGui::TextUnformatted("The X-Plane SDK from Laminar Research");
}

} // Namespace xmidictrl
