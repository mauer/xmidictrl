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

// Font Awesome
#include <IconsFontAwesome6.h>

// XMidiCtrl
#include "plugin.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile_window::profile_window(text_logger &in_log, xplane &in_xp, profile &in_profile)
    : imgui_window(in_log, in_xp, 1200, 750),
      m_profile(in_profile)
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
    if (ImGui::BeginTabBar("PROFILE_TAB")) {
        create_tab_general();
        create_tab_errors_warnings();

        ImGui::EndTabBar();
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create tab for general information
 */
void profile_window::create_tab_general()
{
    if (ImGui::BeginTabItem("General Information")) {
        ImGui::NewLine();

        ImGui::Text("AIRCRAFT INFORMATION");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("ICAO:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().current_aircraft_icao().c_str());

        ImGui::Text("Description:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().current_aircraft_descr().c_str());

        ImGui::Text("Author:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().current_aircraft_author().c_str());

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::Text("CURRENT PROFILE");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("Title:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_profile.title().data());

        ImGui::Text("Version:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_profile.version().data());

        ImGui::Text("Filename:");
        ImGui::SameLine(150);

        if (m_profile.filename().empty())
            ImGui::TextColored(COL_TEXT_VALUE, "<not loaded>");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "%s", m_profile.filename().data());

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::Text("POSSIBLE PROFILE NAMES - AIRCRAFT FOLDER");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("Without prefix:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().get_filename_aircraft_path(filename_prefix::none).data());

        ImGui::Text("ICAO:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().get_filename_aircraft_path(filename_prefix::icao).data());

        ImGui::Text("ACF Name:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().get_filename_aircraft_path(filename_prefix::acf_name).data());

        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::Text("POSSIBLE PROFILE NAMES - PROFILES FOLDER");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("ICAO:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().get_filename_profiles_path(filename_prefix::icao).data());

        ImGui::Text("ACF Name:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().get_filename_profiles_path(filename_prefix::acf_name).data());

        ImGui::NewLine();

        ImGui::Text("Common Profile:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", xp().get_filename_profiles_path(filename_prefix::none).data());

        ImGui::EndTabItem();
    }
}


/**
 * Create tab for errors and warnings
 */
void profile_window::create_tab_errors_warnings()
{
    if (ImGui::BeginTabItem("Errors & Warnings")) {
        ImGui::Text("Errors found:");
        ImGui::SameLine(150);

        if (m_profile.log().has_errors())
            ImGui::TextColored(COL_TEXT_VALUE, "Yes");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "No");

        ImGui::SameLine(ImGui::GetWindowWidth() - 240);

        if (ImGui::Button("  " ICON_FA_ROTATE_RIGHT "  Reload Aircraft Profile  "))
            plugin::instance().load_profile();

        ImGui::Text("Warnings found:");
        ImGui::SameLine(150);

        if (m_profile.log().has_warnings())
            ImGui::TextColored(COL_TEXT_VALUE, "Yes");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "No");

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableTextMessages", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);
        ImGui::TableSetupColumn("Message Date/Time", ImGuiTableColumnFlags_WidthFixed, 200);
        ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (size_t i = 0; i < m_profile.log().count(); i++) {
            auto msg = m_profile.log().message(static_cast<int>(i));

            if (msg == nullptr)
                continue;

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->time.c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->get_log_level_text().c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->text.c_str());
        }

        ImGui::EndTable();
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

} // Namespace xmidictrl