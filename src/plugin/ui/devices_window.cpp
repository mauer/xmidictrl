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

#include "devices_window.h"

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
devices_window::devices_window(std::shared_ptr<xplane> xp)
    : ImGuiWindow(xp, 700, 350)
{
    set_title(std::string(XMIDICTRL_NAME) + " - MIDI Devices");
}


/**
 * Destructor
 */
devices_window::~devices_window() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void devices_window::create_widgets()
{
    ImGui::Text("INBOUND PORTS");
    ImGui::Separator();
    ImGui::NewLine();

    try {
        if (m_midi_in == nullptr)
            m_midi_in = std::make_unique<RtMidiIn>();

        ImGui::BeginTable("tableInbound", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);

        ImGui::TableSetupColumn("Port No.", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("MIDI Device", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        // check inbound ports
        unsigned int port_count = m_midi_in->getPortCount();

        for (unsigned int i = 0; i < port_count; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%i", i);
            ImGui::TableNextColumn();
            ImGui::Text("%s", m_midi_in->getPortName(i).c_str());
        }

        ImGui::EndTable();
    } catch (RtMidiError &error) {
        ImGui::Text("Error: ");
        ImGui::TextWrapped("%s", error.what());
    }

    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("OUTBOUND PORTS");
    ImGui::Separator();
    ImGui::NewLine();

    try {
        if (m_midi_out == nullptr)
            m_midi_out = std::make_unique<RtMidiOut>();

        ImGui::BeginTable("tableOutbound", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable);

        ImGui::TableSetupColumn("Port No.", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("MIDI Device", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        // check outbound ports
        unsigned int portCount = m_midi_out->getPortCount();

        for (unsigned int i = 0; i < portCount; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%i", i);
            ImGui::TableNextColumn();
            ImGui::Text("%s", m_midi_out->getPortName(i).c_str());
        }

        ImGui::EndTable();
    } catch (RtMidiError &error) {
        ImGui::Text("Error: ");
        ImGui::TextWrapped("%s", error.what());
    }
}

} // Namespace xmidictrl