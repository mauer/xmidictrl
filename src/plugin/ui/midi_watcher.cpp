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

#include "midi_watcher.h"

// Font Awesome
#include <IconsFontAwesome6.h>

// XMidiCtrl
#include "midi_message.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
midi_watcher::midi_watcher(text_logger& in_text_log,
                           midi_logger& in_midi_log,
                           environment& in_env)
    : imgui_window(in_text_log, in_env, 1400, 700),
      m_midi_log(in_midi_log)
{
    set_title(std::string(XMIDICTRL_NAME) + " - MIDI Watcher");

    m_midi_msg_flags = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort;
    if (env().settings().default_midi_sort() == sort_mode::ascending)
        m_midi_msg_flags = m_midi_msg_flags | ImGuiTableColumnFlags_PreferSortAscending;
    else
        m_midi_msg_flags = m_midi_msg_flags | ImGuiTableColumnFlags_PreferSortDescending;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void midi_watcher::create_widgets()
{
    ImGui::Text("MIDI Logging:");
    ImGui::SameLine(150);

    if (env().settings().log_midi())
        ImGui::TextColored(value_color(), "Enabled (%i)", static_cast<int>(m_midi_log.count()));
    else
        ImGui::TextColored(value_color(), "Disabled");

    ImGui::SameLine(ImGui::GetWindowWidth() - 190);

    if (ImGui::Button(UI_SPACER_2 ICON_FA_TRASH_CAN UI_SPACER_2 "Clear Messages" UI_SPACER_2))
        m_midi_log.clear();

    ImGui::NewLine();
    ImGui::TextColored(title_color(), "%s", "MESSAGES");
    ImGui::Separator();

    ImGui::BeginChild("TEXT_TABLE");

    if (ImGui::BeginTable("tableMidiMessages", 10,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable
                          | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Date/Time", m_midi_msg_flags, 200);
        ImGui::TableSetupColumn("Direction", ImGuiTableColumnFlags_WidthFixed, 90);
        ImGui::TableSetupColumn("Port", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 160);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data 1", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data 2", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Mappings", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableSetupColumn("Log", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Raw Data", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
            if (sort_specs->SpecsDirty && sort_specs->SpecsCount > 0) {
                auto spec = sort_specs->Specs[0];
                if (spec.ColumnIndex == 0 && spec.SortDirection == ImGuiSortDirection_Ascending)
                    m_midi_sort_mode = sort_mode::ascending;
                else
                    m_midi_sort_mode = sort_mode::descending;
            }
        }

        if (m_midi_sort_mode == sort_mode::ascending) {
            for (size_t i = 0; i < m_midi_log.count(); i++) {
                auto msg = m_midi_log.message(static_cast<int>(i));
                add_midi_row(msg);
            }
        } else if (m_midi_log.count() > 0) {
            for (size_t i = m_midi_log.count() - 1; i > 0; i--) {
                auto msg = m_midi_log.message(static_cast<int>(i));
                add_midi_row(msg);
            }
        }
    }

    ImGui::EndTable();
    ImGui::EndChild();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------


/**
 * Add a MIDI message to the table
 */
void midi_watcher::add_midi_row(midi_message* in_msg)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->time().c_str());

    ImGui::TableNextColumn();
    if (in_msg->direction() == midi_direction::in)
        draw_icon(ICON_FA_ARROW_RIGHT_TO_BRACKET, "Inbound");
    else
        draw_icon(ICON_FA_ARROW_RIGHT_FROM_BRACKET, "Outbound");

    ImGui::TableNextColumn();
    ImGui::Text("%i", in_msg->port());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->type_as_text().c_str());

    ImGui::TableNextColumn();
    if (in_msg->channel() != MIDI_NONE)
        ImGui::Text("%i", in_msg->channel());

    ImGui::TableNextColumn();
    if (in_msg->data_1() != MIDI_NONE)
        ImGui::Text("%s", in_msg->data_1_as_text(env().settings().note_name()).c_str());

    ImGui::TableNextColumn();
    if (in_msg->data_2() != MIDI_NONE)
        ImGui::Text("%i", in_msg->data_2());

    ImGui::TableNextColumn();
    size_t map_count = in_msg->mapping_count();
    if (map_count == 1)
        draw_icon(ICON_FA_CHECK, in_msg->mapping_text().c_str());
    else if (map_count > 1)
        draw_icon(ICON_FA_CHECK_DOUBLE, in_msg->mapping_text().c_str());

    ImGui::TableNextColumn();
    if (in_msg->log().has_errors())
        draw_icon(ICON_FA_SQUARE_XMARK, in_msg->log().messages_as_text().c_str());
    else if (in_msg->log().has_warnings())
        draw_icon(ICON_FA_TRIANGLE_EXCLAMATION, in_msg->log().messages_as_text().c_str());
    else if (in_msg->log().count() > 0)
        draw_icon(ICON_FA_CIRCLE_CHECK, in_msg->log().messages_as_text().c_str());
    else if (in_msg->log().count() == 0 && map_count > 0)
        draw_icon(ICON_FA_CIRCLE_CHECK);

    ImGui::TableNextColumn();
    if (in_msg->data_2() != MIDI_NONE)
        ImGui::Text("Status = %i | Data 1 = %i | Data 2 = %i", in_msg->status(), in_msg->data_1(), in_msg->data_2());
    else
        ImGui::Text("Status = %i | Data 1 = %i", in_msg->status(), in_msg->data_1());
}


/**
 * Draw an icon with a popup text
 */
void midi_watcher::draw_icon(const char* in_icon, std::string_view in_text)
{
    ImGui::TextUnformatted(in_icon);

    if (!in_text.empty()) {
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();

            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 100.0f);
            ImGui::TextUnformatted(in_text.data());
            ImGui::PopTextWrapPos();

            ImGui::EndTooltip();
        }
    }
}

} // Namespace xmidictrl