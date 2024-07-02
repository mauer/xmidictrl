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

#include "devices_window.h"

// Standard
#include <string>

// fmt
#include "fmt/format.h"

// Font Awesome
#include <IconsFontAwesome6.h>
#include <IconsFontAwesome6Brands.h>

// hidapi
#include "hidapi.h"

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
	m_hid_devices = read_hid_devices();
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
 * Returns all list of all connected HID devices
 * @return List of all HID devices with detailed information
 */
std::map<std::string, std::unique_ptr<hid_device_data>> devices_window::read_hid_devices()
{
	auto devices = std::map<std::string, std::unique_ptr<hid_device_data>>();
	struct hid_device_info* hid_devs;

	hid_devs = hid_enumerate(0x0, 0x0);

	for (; hid_devs; hid_devs = hid_devs->next) {
		if (hid_devs->interface_number == -1)
			continue;

		auto dev = std::make_unique<hid_device_data>();

		// convert manufacturer string
		auto ws = std::wstring(hid_devs->manufacturer_string);
		dev->manufacturer_string = std::string(ws.begin(), ws.end());

		// convert product string
		ws = std::wstring(hid_devs->product_string);
		dev->product_string = std::string(ws.begin(), ws.end());

		if (dev->product_string.empty())
			continue;

		dev->vendor_id = hid_devs->vendor_id;
		dev->product_id = hid_devs->product_id;
		dev->serial_no = hid_devs->serial_number;

		dev->usb_interface_no = hid_devs->interface_number;

		std::string key = std::to_string(dev->vendor_id) + std::to_string(dev->product_id);

		devices.try_emplace(key, std::move(dev));
	}

	hid_free_enumeration(hid_devs);

	return devices;
}


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

		if (ImGui::BeginTable("tableInbound", 6, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable)) {
			ImGui::TableSetupColumn("Vendor ID", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("Product ID", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("Serial No.", ImGuiTableColumnFlags_WidthFixed, 100);
			ImGui::TableSetupColumn("USB", ImGuiTableColumnFlags_WidthFixed, 30);
			ImGui::TableSetupColumn("Product Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Manufacturer Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			for (const auto& dev: m_hid_devices) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{:#06x}", dev.second->product_id).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{:#06x}", dev.second->vendor_id).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(reinterpret_cast<const char*>(dev.second->serial_no));
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{}", dev.second->usb_interface_no).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{}", dev.second->product_string).data());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(fmt::format("{}", dev.second->manufacturer_string).data());
			}
		}
		ImGui::EndTable();

		ImGui::EndTabItem();
	}
}

} // Namespace xmidictrl