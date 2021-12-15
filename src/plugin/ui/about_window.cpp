//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#include "about_window.h"

// Standard
#include <string>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
about_window::about_window(std::shared_ptr<xplane> xp)
    : ImGuiWindow(xp, 850, 450)
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
    ImGui::TextUnformatted(XMIDICTRL_NAME " - " XMIDICTRL_DESCRIPTION);
    ImGui::NewLine();
    ImGui::TextUnformatted("Version " XMIDICTRL_VERSION_STR);
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::TextUnformatted("Copyright (c) 2021 by Marco Auer");
    ImGui::NewLine();
    ImGui::TextUnformatted(XMIDICTRL_NAME " has been released under the MIT License and is therefore free to use without");
    ImGui::TextUnformatted("any restrictions or limitations. This software is provided without warranty of any kind.");
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
    ImGui::TextUnformatted(
        "Dear ImGui X-Plane Integration from Christopher Collins (https://github.com/xsquawkbox/xsb_public)");
    ImGui::Bullet();
    ImGui::TextUnformatted("The X-Plane SDK from Laminar Research");
}

} // Namespace xmidictrl