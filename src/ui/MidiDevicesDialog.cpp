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
#include "MidiDevicesDialog.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
MidiDevicesDialog::MidiDevicesDialog() :
ImGuiWindow(700, 350)
{
    LOG_DEBUG << "Create the midi devices dialog" << LOG_END

    setTitle("Midi Devices");
}


/**
 * Destructor
 */
MidiDevicesDialog::~MidiDevicesDialog() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void MidiDevicesDialog::createWidgets() {
    //ImGui::PushFont

    ImGui::Text("INBOUND PORTS");
    ImGui::Separator();
    ImGui::NewLine();

    try {
        m_midiIn = std::make_unique<RtMidiIn>();

        ImGui::BeginTable("tableInbound", 2);

        ImGui::TableSetupColumn("Port No.");
        ImGui::TableSetupColumn("MIDI Device");
        ImGui::TableHeadersRow();

        // check inbound ports
        unsigned int portCount = m_midiIn->getPortCount();

        for (unsigned int i = 0; i < portCount; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%i", i);
            ImGui::TableNextColumn();
            ImGui::Text("%s", m_midiIn->getPortName(i).c_str());
        }

        ImGui::EndTable();
    } catch (RtMidiError& midiError) {
        ImGui::Text("Error: ");
        ImGui::TextWrapped("%s", midiError.getMessage().c_str());
    }

    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("OUTBOUND PORTS");
    ImGui::Separator();
    ImGui::NewLine();

    try {
        m_midiOut = std::make_unique<RtMidiOut>();

        ImGui::BeginTable("tableOutbound", 2);

        ImGui::TableSetupColumn("Port No.");
        ImGui::TableSetupColumn("MIDI Device");
        ImGui::TableHeadersRow();

        // check outbound ports
        unsigned int portCount = m_midiOut->getPortCount();

        for (unsigned int i = 0; i < portCount; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%i", i);
            ImGui::TableNextColumn();
            ImGui::Text("%s", m_midiOut->getPortName(i).c_str());
        }

        ImGui::EndTable();
    } catch (RtMidiError& midiError) {
        ImGui::Text("Error: ");
        ImGui::TextWrapped("%s", midiError.getMessage().c_str());
    }
}

} // Namespace XMidiCtrl