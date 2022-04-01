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

#include <utility>

// XMidiCtrl
#include "conversions.h"
#include "midi_message.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
messages_window::messages_window(text_logger *in_text_log, midi_logger *in_midi_log, xplane *in_xp)
    : ImGuiWindow(in_text_log, in_xp, 1400, 700),
      m_midi_log(in_midi_log)
{
    set_title(std::string(XMIDICTRL_NAME) + " - Messages");
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
        if (m_log->debug_mode())
            ImGui::TextColored(COL_TEXT_VALUE, "Enabled");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "Disabled");
        ImGui::SameLine(ImGui::GetWindowWidth() - 150);

        if (ImGui::Button("Clear Messages"))
            m_log->clear();

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableTextMessages", 3,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable);
        ImGui::TableSetupColumn("Message Date/Time",
                                ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort,
                                200);
        ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        sort_mode mode = sort_mode::descending;
        if (ImGuiTableSortSpecs *sort_specs = ImGui::TableGetSortSpecs()) {
            if (sort_specs->SpecsCount > 0) {
                auto spec = sort_specs->Specs[0];
                if (spec.ColumnIndex == 0 && spec.SortDirection == ImGuiSortDirection_Ascending)
                    mode = sort_mode::ascending;
            }
        }

        if (mode == sort_mode::ascending) {
            for (unsigned int i = 0; i < m_log->count(); i++) {
                auto msg = m_log->message(i);

                if (msg != nullptr)
                    add_text_row(msg);
            }
        } else {
            for (unsigned int i = m_log->count() - 1; i > 0; i--) {
                auto msg = m_log->message(i);

                if (msg != nullptr)
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

        if (m_midi_log->state())
            ImGui::TextColored(COL_TEXT_VALUE, "Enabled");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "Disabled");

        ImGui::SameLine(ImGui::GetWindowWidth() - 150);

        if (ImGui::Button("Clear Messages"))
            m_midi_log->clear();

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableMidiMessages", 10,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable);
        ImGui::TableSetupColumn("Date/Time",
                                ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort,
                                200);
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

        sort_mode mode = sort_mode::descending;
        if (ImGuiTableSortSpecs *sort_specs = ImGui::TableGetSortSpecs()) {
            if (sort_specs->SpecsCount > 0) {
                auto spec = sort_specs->Specs[0];
                if (spec.ColumnIndex == 0 && spec.SortDirection == ImGuiSortDirection_Ascending)
                    mode = sort_mode::ascending;
            }
        }

        if (mode == sort_mode::ascending) {
            for (unsigned int i = 0; i < m_midi_log->count(); i++) {
                auto msg = m_midi_log->message(i);

                if (msg != nullptr)
                    add_midi_row(msg);
            }
        } else {
            for (unsigned int i = m_midi_log->count() - 1; i < 0; i--) {
                auto msg = m_midi_log->message(i);

                if (msg != nullptr)
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
void messages_window::add_text_row(text_log_msg *msg)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(msg->time.c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(msg->get_log_level_text().c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(msg->text.c_str());
}


/**
 * Add a MIDI message to the table
 */
void messages_window::add_midi_row(midi_message *msg)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(msg->time().c_str());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(msg->direction_as_text().c_str());

    ImGui::TableNextColumn();
    ImGui::Text("%i", msg->port());

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(msg->type_as_text().c_str());

    ImGui::TableNextColumn();
    if (msg->channel() != MIDI_NONE)
        ImGui::Text("%i", msg->channel());

    ImGui::TableNextColumn();
    if (msg->data_1() != MIDI_NONE)
        ImGui::Text("%i", msg->data_1());

    ImGui::TableNextColumn();
    if (msg->data_2() != MIDI_NONE)
        ImGui::Text("%i", msg->data_2());

    ImGui::TableNextColumn();
    ImGui::Text("%zu", msg->mapping_count());
    if (msg->mapping_count() > 0) {
        ImGui::SameLine(10);
        info_marker(msg->mappings_as_text().c_str());
    }

    ImGui::TableNextColumn();
    if (msg->log()->count() > 0) {
        info_marker(msg->log()->message(0)->text.c_str());
    }

    ImGui::TableNextColumn();
    if (msg->data_2() != MIDI_NONE)
        ImGui::Text("Status = %i | Data 1 = %i | Data 2 = %i", msg->status(), msg->data_1(), msg->data_2());
    else
        ImGui::Text("Status = %i | Data 1 = %i", msg->status(), msg->data_1());
}


/**
 * Info marker
 */
// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
void messages_window::info_marker(std::string_view text)
{
    ImGui::TextDisabled("[i]");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 100.0f);
        ImGui::TextUnformatted(text.data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

} // Namespace xmidictrl