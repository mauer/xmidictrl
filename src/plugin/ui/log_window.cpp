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

#include "log_window.h"

// Font Awesome
#include <IconsFontAwesome6.h>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
log_window::log_window(text_logger& in_text_log, environment& in_env)
    : imgui_window(in_text_log, in_env, 1400, 700)
{
    set_title(std::string(XMIDICTRL_NAME) + " - Log Messages");

    m_log_msg_flags = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort;
    if (env().settings().default_text_sort() == sort_mode::ascending)
        m_log_msg_flags = m_log_msg_flags | ImGuiTableColumnFlags_PreferSortAscending;
    else
        m_log_msg_flags = m_log_msg_flags | ImGuiTableColumnFlags_PreferSortDescending;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void log_window::create_widgets()
{
    ImGui::Text("Debug Mode:");
    ImGui::SameLine(150);
    if (log().debug_mode())
        ImGui::TextColored(value_color(), "Enabled (%i)", static_cast<int>(log().count()));
    else
        ImGui::TextColored(value_color(), "Disabled");
    ImGui::SameLine(ImGui::GetWindowWidth() - 190);

    if (ImGui::Button(UI_SPACER_2 ICON_FA_TRASH_CAN UI_SPACER_2 "Clear Messages" UI_SPACER_2))
        log().clear();

    ImGui::NewLine();
    ImGui::TextColored(title_color(), "%s", "MESSAGES");
    ImGui::Separator();

    ImGui::BeginChild("LOG_TABLE");

    ImGui::BeginTable("tableLogMessages", 3,
                      ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable);

    ImGui::TableSetupColumn("Date/Time", m_log_msg_flags, 200);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 120);
    ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
        if (sort_specs->SpecsDirty && sort_specs->SpecsCount > 0) {
            auto spec = sort_specs->Specs[0];
            if (spec.ColumnIndex == 0 && spec.SortDirection == ImGuiSortDirection_Ascending)
                m_log_sort_mode = sort_mode::ascending;
            else
                m_log_sort_mode = sort_mode::descending;
        }
    }

    if (m_log_sort_mode == sort_mode::ascending) {
        for (size_t i = 0; i < log().count(); i++) {
            auto msg = log().message(static_cast<int>(i));
            add_log_row(msg);
        }
    } else if (log().count() > 0) {
        for (size_t i = log().count() - 1; i > 0; i--) {
            auto msg = log().message(static_cast<int>(i));
            add_log_row(msg);
        }
    }

    ImGui::EndTable();
    ImGui::EndChild();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a log message to the table
 */
void log_window::add_log_row(text_log_msg* in_msg)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->time.c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->get_log_level_text().c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->text.c_str());
}

} // Namespace xmidictrl