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

#include "messages_window.h"

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
messages_window::messages_window(text_logger &in_text_log,
                                 midi_logger &in_midi_log,
                                 xplane &in_xp,
                                 settings &in_settings)
    : imgui_window(in_text_log, in_xp, 1400, 700),
      m_settings(in_settings),
      m_midi_log(in_midi_log)
{
    set_title(std::string(XMIDICTRL_NAME) + " - Messages");

    m_text_msg_flags = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort;
    if (m_settings.default_text_sort() == sort_mode::ascending)
        m_text_msg_flags = m_text_msg_flags | ImGuiTableColumnFlags_PreferSortAscending;
    else
        m_text_msg_flags = m_text_msg_flags | ImGuiTableColumnFlags_PreferSortDescending;

    m_midi_msg_flags = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort;
    if (m_settings.default_midi_sort() == sort_mode::ascending)
        m_midi_msg_flags = m_midi_msg_flags | ImGuiTableColumnFlags_PreferSortAscending;
    else
        m_midi_msg_flags = m_midi_msg_flags | ImGuiTableColumnFlags_PreferSortDescending;
}


/**
 * Destructor
 */
messages_window::~messages_window() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void messages_window::create_widgets()
{
    ImGui::BeginTabBar("MESSAGES_TAB");

    create_tab_text_msg();
    create_tab_midi_msg();

    ImGui::EndTabBar();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create tab for text messages
 */
void messages_window::create_tab_text_msg()
{
    if (ImGui::BeginTabItem("General Log")) {
        ImGui::Text("Debug Mode:");
        ImGui::SameLine(150);
        if (m_log.debug_mode())
            ImGui::TextColored(COL_TEXT_VALUE, "Enabled");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "Disabled");
        ImGui::SameLine(ImGui::GetWindowWidth() - 200);

        if (ImGui::Button("  " ICON_FA_TRASH_CAN " Clear Messages  "))
            m_log.clear();

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableTextMessages", 3,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable);

        ImGui::TableSetupColumn("Message Date/Time", m_text_msg_flags,200);
        ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        if (ImGuiTableSortSpecs *sort_specs = ImGui::TableGetSortSpecs()) {
            if (sort_specs->SpecsDirty && sort_specs->SpecsCount > 0) {
                auto spec = sort_specs->Specs[0];
                if (spec.ColumnIndex == 0 && spec.SortDirection == ImGuiSortDirection_Ascending)
                    m_text_sort_mode = sort_mode::ascending;
                else
                    m_text_sort_mode = sort_mode::descending;
            }
        }

        if (m_text_sort_mode == sort_mode::ascending) {
            for (size_t i = 0; i < m_log.count(); i++) {
                auto msg = m_log.message(static_cast<int>(i));
                add_text_row(msg);
            }
        } else if (m_log.count() > 0) {
            for (size_t i = m_log.count() - 1; i > 0; i--) {
                auto msg = m_log.message(static_cast<int>(i));
                add_text_row(msg);
            }
        }

        ImGui::EndTable();
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}


/**
 * Create tab for midi messages
 */
void messages_window::create_tab_midi_msg()
{
    if (ImGui::BeginTabItem("MIDI Messages")) {
        ImGui::Text("MIDI Logging:");
        ImGui::SameLine(150);

        if (m_midi_log.state())
            ImGui::TextColored(COL_TEXT_VALUE, "Enabled");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "Disabled");

        ImGui::SameLine(ImGui::GetWindowWidth() - 200);

        if (ImGui::Button("  " ICON_FA_TRASH_CAN " Clear Messages  "))
            m_midi_log.clear();

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableMidiMessages", 10,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable);
        ImGui::TableSetupColumn("Date/Time", m_midi_msg_flags, 200);
        ImGui::TableSetupColumn("Direction", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableSetupColumn("Port", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Velocity", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Mappings", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableSetupColumn("Log", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Raw Data", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        if (ImGuiTableSortSpecs *sort_specs = ImGui::TableGetSortSpecs()) {
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

        ImGui::EndTable();
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}


/**
 * Add a text message to the table
 */
void messages_window::add_text_row(text_log_msg *in_msg)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->time.c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->get_log_level_text().c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->text.c_str());
}


/**
 * Add a MIDI message to the table
 */
void messages_window::add_midi_row(midi_message *in_msg)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->time().c_str());

    ImGui::TableNextColumn();
    if (in_msg->direction() == midi_direction::in)
        draw_icon(ICON_FA_ARROW_LEFT, "Inbound");
    else
        draw_icon(ICON_FA_ARROW_RIGHT, "Outbound");

    ImGui::TableNextColumn();
    ImGui::Text("%i", in_msg->port());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(in_msg->type_as_text().c_str());

    ImGui::TableNextColumn();
    if (in_msg->channel() != MIDI_NONE)
        ImGui::Text("%i", in_msg->channel());

    ImGui::TableNextColumn();
    if (in_msg->data_1() != MIDI_NONE)
        ImGui::Text("%i", in_msg->data_1());

    ImGui::TableNextColumn();
    if (in_msg->data_2() != MIDI_NONE)
        ImGui::Text("%i", in_msg->data_2());

    ImGui::TableNextColumn();
    size_t map_count = in_msg->mapping_count();
    if (map_count == 1)
        draw_icon(ICON_FA_CHECK, in_msg->mappings_as_string().c_str());
    else if (map_count > 1)
        draw_icon(ICON_FA_CHECK_DOUBLE, in_msg->mappings_as_string().c_str());

    ImGui::TableNextColumn();
    if (in_msg->log().has_errors())
        draw_icon(ICON_FA_BAN, in_msg->log().messages_as_text().c_str());
    else if (in_msg->log().has_warnings())
        draw_icon(ICON_FA_TRIANGLE_EXCLAMATION, in_msg->log().messages_as_text().c_str());
    else if (in_msg->log().count() > 0)
        draw_icon(ICON_FA_CIRCLE_CHECK, in_msg->log().messages_as_text().c_str());
    else if (in_msg->log().count() == 0 && map_count > 0)
        draw_icon(ICON_FA_CIRCLE_CHECK, "");

    ImGui::TableNextColumn();
    if (in_msg->data_2() != MIDI_NONE)
        ImGui::Text("Status = %i | Data 1 = %i | Data 2 = %i", in_msg->status(), in_msg->data_1(), in_msg->data_2());
    else
        ImGui::Text("Status = %i | Data 1 = %i", in_msg->status(), in_msg->data_1());
}


/**
 * Draw an icon with a popup text
 */
void messages_window::draw_icon(const char *in_icon, std::string_view in_text)
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