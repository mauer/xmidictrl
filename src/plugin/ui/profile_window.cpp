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

#include "profile_window.h"

// Standard
#include <string>
#include <utility>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile_window::profile_window(std::shared_ptr<xplane> xp, std::shared_ptr<profile> prf)
    : ImGuiWindow(xp, 1200, 550),
      m_prf(std::move(prf))
{
    set_title(std::string(XMIDICTRL_NAME) + " - Aircraft Profile");
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void profile_window::create_widgets()
{
    ImGui::Text("AIRCRAFT INFORMATION");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("ICAO:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_xp->current_aircraft_icao().c_str());

    ImGui::Text("Description:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_xp->current_aircraft_descr().c_str());

    ImGui::Text("Author:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_xp->current_aircraft_author().c_str());

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("CURRENT PROFILE");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("Title:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->title().data());

    ImGui::Text("Version:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->version().data());

    ImGui::Text("Filename:");
    ImGui::SameLine(150);

    if (m_prf->filename().empty())
        ImGui::TextColored(COL_TEXT_VALUE, "<not loaded>");
    else
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->filename().data());

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("POSSIBLE PROFILE NAMES");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("Aircraft folder:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->get_filename_aircraft_path().data());

    ImGui::NewLine();

    ImGui::Text("ICAO and Author:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->get_filename_profiles_path(true, true).data());

    ImGui::Text("ICAO only:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->get_filename_profiles_path(true, false).data());

    ImGui::NewLine();

    ImGui::Text("Generic Profile:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_prf->get_filename_profiles_path(false, false).data());
}

} // Namespace xmidictrl