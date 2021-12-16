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