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