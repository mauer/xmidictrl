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

// XMidiCtrl
#include "Logger.h"
#include "MessagesDialog.h"
#include "Utils.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MessagesDialog::MessagesDialog()
        : ImGuiWindow(1200, 700) {
    LOG_DEBUG << "Create the messages dialog" << LOG_END

    setTitle("Messages");
}


/**
 * Destructor
 */
MessagesDialog::~MessagesDialog() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void MessagesDialog::createWidgets() {
    ImGui::Text("Current Logging Level: %s", Utils::getLogLevelText(Logger::Instance().logLevel()).c_str());
    ImGui::SameLine(ImGui::GetWindowWidth()-250);

    if (ImGui::Button("Clear Messages"))
        Logger::Instance().messages()->clear();

    ImGui::NewLine();

    ImGui::Text("LOGGING MESSAGES");
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::BeginTable("tableMessages", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);
    ImGui::TableSetupColumn("Message Date/Time", ImGuiTableColumnFlags_WidthFixed, 200);
    ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
    ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    for (int i = Logger::Instance().messages()->size() - 1; i >= 0; i--) {
        Message::ptr message = Logger::Instance().messages()->message(i);

        if (message == nullptr)
            continue;

        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text("%s", message->time.c_str());

        ImGui::TableNextColumn();
        ImGui::Text("%s", Utils::getMessageTypeText(message->type).c_str());

        ImGui::TableNextColumn();
        ImGui::Text("%s", message->text.c_str());
    }

    ImGui::EndTable();
}

} // Namespace XMidiCtrl