//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

// fmt
#include "fmt/format.h"

// Font Awesome
#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6Brands.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
devices_window::devices_window(text_logger& in_log, environment& in_env)
	: imgui_window(in_log, in_env, 700, 350)
{
	set_title(std::string(XMIDICTRL_NAME) + " - Devices");

	// read all hid devices and store them
	m_hid_devices = hid::read_devices();
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void devices_window::create_widgets()
{
	if (ImGui::BeginTabBar("DEVICES_TAB")) {
		create_tab_midi();
		create_tab_hid();

		ImGui::EndTabBar();
	}
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create tab page for MIDI devices
 */
void devices_window::create_tab_midi()
{
	if (ImGui::BeginTabItem("MIDI")) {
		ImGui::TextColored(title_color(), "%s", UI_SPACER_2 ICON_FA_ARROW_RIGHT_TO_BRACKET UI_SPACER_3 "INBOUND PORTS");
		ImGui::Separator();

		try {
			if (m_midi_in == nullptr)
				m_midi_in = std::make_unique<RtMidiIn>();

			if (ImGui::BeginTable("tableInbound", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable)) {
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
			}
			ImGui::EndTable();
		} catch (RtMidiError& error) {
			ImGui::Text("Error: ");
			ImGui::TextWrapped("%s", error.what());
		}

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();

		ImGui::TextColored(title_color(),
						   "%s",
						   UI_SPACER_2 ICON_FA_ARROW_RIGHT_FROM_BRACKET UI_SPACER_3 "OUTBOUND PORTS");
		ImGui::Separator();

		try {
			if (m_midi_out == nullptr)
				m_midi_out = std::make_unique<RtMidiOut>();

			if (ImGui::BeginTable("tableOutbound", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable)) {
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
			}
			ImGui::EndTable();
		} catch (RtMidiError& error) {
			ImGui::Text("Error: ");
			ImGui::TextWrapped("%s", error.what());
		}

		ImGui::EndTabItem();
	}
}


/**
 * Create tab page for HID devices
 */
void devices_window::create_tab_hid()
{
	if (ImGui::BeginTabItem("HID")) {
		ImGui::TextColored(title_color(), "%s", UI_SPACER_2 ICON_FA_USB UI_SPACER_3 "HID DEVICES");
		ImGui::Separator();

		if (ImGui::BeginTable("tableInbound", 4, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable)) {
			ImGui::TableSetupColumn("Vendor ID", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("Product ID", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("Product Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Manufacturer Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			for (const auto& dev: m_hid_devices) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{:#06x}", dev->product_id).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{:#06x}", dev->vendor_id).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{}", dev->product_string).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{}", dev->manufacturer_string).data());
			}
		}
		ImGui::EndTable();

		ImGui::EndTabItem();
	}
}

} // Namespace xmidictrl