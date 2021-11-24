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
messages_window::messages_window()
    : ImGuiWindow(1400, 700)
{
    setTitle("Messages");
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
void messages_window::createWidgets()
{
    ImGui::Text("Current Log Level: %s", utils::get_log_level_Text(logger::instance().get_log_Level()).c_str());
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);

    if (ImGui::Button("Clear Messages"))
        logger::instance().messages()->clear();

    ImGui::NewLine();

    ImGui::Text("LOGGING MESSAGES");
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::BeginTable("tableMessages", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);
    ImGui::TableSetupColumn("Message Date/Time", ImGuiTableColumnFlags_WidthFixed, 200);
    ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
    ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    for (unsigned int i = logger::instance().messages()->size() - 1; i >= 0; i--) {
        std::shared_ptr<message> msg = logger::instance().messages()->get_message(i);

        if (msg == nullptr)
            continue;

        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text("%s", msg->time.c_str());

        ImGui::TableNextColumn();
        ImGui::Text("%s", utils::get_message_type_text(msg->type).c_str());

        ImGui::TableNextColumn();
        ImGui::Text("%s", msg->text.c_str());
    }

    ImGui::EndTable();
}

} // Namespace xmidictrl