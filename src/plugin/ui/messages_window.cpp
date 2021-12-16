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

// XMidiCtrl
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
messages_window::messages_window(std::shared_ptr<xplane> xp)
    : ImGuiWindow(xp, 1400, 700)
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
    if (ImGui::BeginTabItem("Text Messages")) {
        ImGui::Text("Logging Level:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s",
                           utils::log_level_as_text(logger::instance().get_log_Level()).c_str());
        ImGui::SameLine(ImGui::GetWindowWidth() - 150);

        if (ImGui::Button("Clear Messages"))
            logger::instance().clear_text_messages();

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableTextMessages", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);
        ImGui::TableSetupColumn("Message Date/Time", ImGuiTableColumnFlags_WidthFixed, 200);
        ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (unsigned int i = 0; i < logger::instance().count_text_messages(); i++) {
            auto msg = logger::instance().get_text_message(i);

            if (msg == nullptr)
                continue;

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->time.c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->type.c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->text.c_str());
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

        if (logger::instance().get_log_midi())
            ImGui::TextColored(COL_TEXT_VALUE, "Enabled");
        else
            ImGui::TextColored(COL_TEXT_VALUE, "Disabled");

        ImGui::SameLine(ImGui::GetWindowWidth() - 150);

        if (ImGui::Button("Clear Messages"))
            logger::instance().clear_midi_messages();

        ImGui::NewLine();
        ImGui::Text("MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        ImGui::BeginTable("tableMidiMessages", 7, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);
        ImGui::TableSetupColumn("Date/Time", ImGuiTableColumnFlags_WidthFixed, 200);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Port", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Control Change", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Velocity", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Raw Data", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (unsigned int i = 0; i < logger::instance().count_midi_messages(); i++) {
            auto msg = logger::instance().get_midi_message(i);

            if (msg == nullptr)
                continue;

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->time.c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%s", msg->type.c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%i", msg->port);

            ImGui::TableNextColumn();
            ImGui::Text("%i", msg->status - OFFSET_MIDI_CHANNEL_STATUS);

            ImGui::TableNextColumn();
            ImGui::Text("%i", msg->data);

            ImGui::TableNextColumn();
            ImGui::Text("%i", msg->velocity);

            ImGui::TableNextColumn();
            ImGui::Text("Status = %i, Data = %i, Velocity = %i", msg->status, msg->data, msg->velocity);
        }

        ImGui::EndTable();
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

} // Namespace xmidictrl