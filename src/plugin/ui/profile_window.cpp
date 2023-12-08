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

#include "profile_window.h"

// Standard
#include <string>

// Font Awesome
#include <IconsFontAwesome6.h>

// XMidiCtrl
#include "conversions.h"
#include "plugin.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
profile_window::profile_window(text_logger& in_log, environment& in_env, profile& in_profile)
    : imgui_window(in_log, in_env, 1600, 750),
      m_profile(in_profile)
{
    set_title(std::string(XMIDICTRL_NAME) + " - Aircraft Profile");
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void profile_window::create_widgets()
{
    if (ImGui::BeginTabBar("PROFILE_TAB")) {
        create_tab_general();
        create_tab_errors_warnings();
        create_tab_devices();

        ImGui::EndTabBar();
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create tab for general information
 */
void profile_window::create_tab_general()
{
    if (ImGui::BeginTabItem("General Information")) {
        ImGui::TextColored(title_color(), "%s", "AIRCRAFT INFORMATION");

        ImGui::SameLine(ImGui::GetWindowWidth() - 240);

        if (ImGui::Button("  " ICON_FA_ROTATE_RIGHT "  Reload Aircraft Profile  "))
            plugin::instance().load_profile();

        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("ICAO:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", env().current_aircraft_icao().c_str());

        ImGui::Text("Description:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", env().current_aircraft_descr().c_str());

        ImGui::Text("Author:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", env().current_aircraft_author().c_str());

        ImGui::Text("ACF Filename:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", env().current_aircraft_acf_name().c_str());

        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::TextColored(title_color(), "%s", "CURRENT PROFILE");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("Title:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.title().data());

        ImGui::Text("Version:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.version().data());

        ImGui::Text("Filename:");
        ImGui::SameLine(180);

        if (m_profile.filename().empty())
            ImGui::TextColored(value_color(), "<not loaded>");
        else
            ImGui::TextColored(value_color(), "%s", m_profile.filename().data());

        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::TextColored(title_color(), "%s", "POSSIBLE PROFILE NAMES - AIRCRAFT FOLDER");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("Without prefix:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.get_filename_aircraft_path(filename_prefix::none).data());

        ImGui::Text("ICAO:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.get_filename_aircraft_path(filename_prefix::icao).data());

        ImGui::Text("ACF Name:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.get_filename_aircraft_path(filename_prefix::acf_name).data());

        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::TextColored(title_color(), "%s", "POSSIBLE PROFILE NAMES - PROFILES FOLDER");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Text("ICAO:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.get_filename_profiles_path(filename_prefix::icao).data());

        ImGui::Text("ACF Name:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.get_filename_profiles_path(filename_prefix::acf_name).data());

        ImGui::NewLine();

        ImGui::Text("Common Profile:");
        ImGui::SameLine(180);
        ImGui::TextColored(value_color(), "%s", m_profile.get_filename_profiles_path(filename_prefix::none).data());

        ImGui::EndTabItem();
    }
}


/**
 * Create tabs for all devices
 */
void profile_window::create_tab_devices()
{
    // check if there are any devices
    if (m_profile.devices().size() == 0)
        return;

    for (auto const& device: m_profile.devices()) {
        if (device != nullptr)
            create_tab_device(*device);
    }
}


/**
 * Create tab for a device
 */
void profile_window::create_tab_device(device& in_device)
{
    std::string tab_title = in_device.settings().name;

    if (ImGui::BeginTabItem(tab_title.data())) {
        create_title("SETTINGS");

        if (in_device.type() == device_type::midi_device) {
            ImGui::Text("Inbound Port:");
            ImGui::SameLine(180);
            ImGui::TextColored(value_color(), "%i", in_device.settings().port_in);

            ImGui::SameLine(400);
            ImGui::Text("Outbound Delay:");
            ImGui::SameLine(600);
            ImGui::TextColored(value_color(), "%0.f", in_device.settings().outbound_delay);

            ImGui::SameLine(800);
            ImGui::Text("Default Encoder Mode:");
            ImGui::SameLine(1000);

            if (in_device.settings().default_enc_mode == encoder_mode::relative)
                ImGui::TextColored(value_color(), "Relative");
            else
                ImGui::TextColored(value_color(), "Range");

            ImGui::Text("Outbound Port:");
            ImGui::SameLine(180);
            ImGui::TextColored(value_color(), "%i", in_device.settings().port_out);

            ImGui::SameLine(400);
            ImGui::Text("Outbound Note Mode:");
            ImGui::SameLine(600);

            if (in_device.settings().note_mode == outbound_note_mode::on)
                ImGui::TextColored(value_color(), "Note On only");
            else
                ImGui::TextColored(value_color(), "Note On/Off");
        }

        ImGui::Text("Sublayer Dataref:");
        ImGui::SameLine(180);

        if (!in_device.settings().sl_dataref.empty())
            ImGui::TextColored(value_color(), "%s", in_device.settings().sl_dataref.c_str());
        else
            ImGui::TextColored(value_color(), "<none>");

        if (in_device.type() == device_type::midi_device) {
            ImGui::SameLine(400);
            ImGui::Text("Outbound Send Mode:");
            ImGui::SameLine(600);

            if (in_device.settings().send_mode == outbound_send_mode::permanent)
                ImGui::TextColored(value_color(), "Permanent");
            else
                ImGui::TextColored(value_color(), "On change");
        }

        ImGui::NewLine();

        // Add mappings
        create_title("MAPPINGS", false);

        // Button for init
        if (m_current_map_page == map_page::init) {
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Text, env().settings().value_color());
        }

        ImGui::SameLine();
        if (ImGui::Button("INIT", ImVec2(400, 0)))
            m_current_map_page = map_page::init;

        if (m_current_map_page == map_page::init) {
            ImGui::PopID();
            ImGui::PopStyleColor();
        }

        // Button for inbound
        if (m_current_map_page == map_page::inbound) {
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Text, env().settings().value_color());
        }

        ImGui::SameLine();
        if (ImGui::Button("INBOUND", ImVec2(400, 0)))
            m_current_map_page = map_page::inbound;

        if (m_current_map_page == map_page::inbound) {
            ImGui::PopID();
            ImGui::PopStyleColor();
        }

        // Button for outbound
        if (m_current_map_page == map_page::outbound) {
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Text, env().settings().value_color());
        }

        ImGui::SameLine();
        if (ImGui::Button("OUTBOUND", ImVec2(400, 0)))
            m_current_map_page = map_page::outbound;

        if (m_current_map_page == map_page::outbound) {
            ImGui::PopID();
            ImGui::PopStyleColor();
        }

        switch (m_current_map_page) {
            using enum map_page;

            case init:
                create_table_mapping_init(in_device);
                break;

            case inbound:
                create_table_mapping_in(in_device);
                break;

            case outbound:
                create_table_mapping_out(in_device);
                break;
        }

        ImGui::EndTabItem();
    }
}


/**
 * Create tab for errors and warnings
 */
void profile_window::create_tab_errors_warnings()
{
    if (ImGui::BeginTabItem("Errors & Warnings")) {
        ImGui::Text("Errors found:");
        ImGui::SameLine(150);

        if (m_profile.log().has_errors())
            ImGui::TextColored(value_color(), "Yes");
        else
            ImGui::TextColored(value_color(), "No");

        ImGui::SameLine(ImGui::GetWindowWidth() - 240);

        if (ImGui::Button("  " ICON_FA_ROTATE_RIGHT "  Reload Aircraft Profile  "))
            plugin::instance().load_profile();

        ImGui::Text("Warnings found:");
        ImGui::SameLine(150);

        if (m_profile.log().has_warnings())
            ImGui::TextColored(value_color(), "Yes");
        else
            ImGui::TextColored(value_color(), "No");

        ImGui::NewLine();
        ImGui::TextColored(title_color(), "MESSAGES");
        ImGui::Separator();

        ImGui::BeginChild("TEXT_TABLE");

        if (ImGui::BeginTable("tableTextMessages", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupColumn("Message Date/Time", ImGuiTableColumnFlags_WidthFixed, 200);
            ImGui::TableSetupColumn("Message Type", ImGuiTableColumnFlags_WidthFixed, 120);
            ImGui::TableSetupColumn("Message Text", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            for (size_t i = 0; i < m_profile.log().count(); i++) {
                auto msg = m_profile.log().message(static_cast<int>(i));

                if (msg == nullptr)
                    continue;

                ImGui::TableNextRow();

                ImGui::TableNextColumn();
                ImGui::Text("%s", msg->time.c_str());

                ImGui::TableNextColumn();
                ImGui::Text("%s", msg->get_log_level_text().c_str());

                ImGui::TableNextColumn();
                ImGui::Text("%s", msg->text.c_str());
            }
        }
        ImGui::EndTable();
        ImGui::EndChild();

        ImGui::EndTabItem();
    }
}


/**
 * Create a title with separator
 */
void profile_window::create_title(std::string_view in_title, bool in_newline)
{
    ImGui::TextColored(title_color(), "%s", in_title.data());
    ImGui::Separator();

    if (in_newline)
        ImGui::NewLine();
}


/**
 * Create a table for all init mappings
 */
void profile_window::create_table_mapping_init(device& in_device)
{
    // Init mappings available for MIDI devices only
    if (in_device.type() != device_type::midi_device)
        return;

    auto& midi_dev = dynamic_cast<midi_device&>(in_device);

    if (ImGui::BeginTable("mapping_init",
                          5,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("No", ImGuiTableColumnFlags_WidthFixed, 40);
        ImGui::TableSetupColumn("Include", ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data 1", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data 2");

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        for (const auto& mapping: midi_dev.mapping_init()) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            // "%03i"
            ImGui::Text("%i", mapping->no());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->include_name().data());

            ImGui::TableNextColumn();
            ImGui::Text("%i", mapping->channel());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->data_1_as_string().data());

            ImGui::TableNextColumn();
            ImGui::Text("%i", mapping->data_2());
        }

        ImGui::EndTable();
    }
}


/**
 * Create a table for all inbound mappings
 */
void profile_window::create_table_mapping_in(device& in_device)
{
    int col_no = 7;

    if (!in_device.settings().sl_dataref.empty())
        col_no++;

    if (!in_device.settings().include.empty())
        col_no++;

    if (ImGui::BeginTable("mapping_in",
                          col_no,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("No", ImGuiTableColumnFlags_WidthFixed, 40);

        if (!in_device.settings().include.empty())
            ImGui::TableSetupColumn("Include", ImGuiTableColumnFlags_WidthFixed, 120);

        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data 1", ImGuiTableColumnFlags_WidthFixed, 80);

        if (!in_device.settings().sl_dataref.empty())
            ImGui::TableSetupColumn("Sublayer", ImGuiTableColumnFlags_WidthFixed, 80);

        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableSetupColumn("Command / Dataref", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();
        for (const auto& [key, mapping]: in_device.mapping_in()) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("%i", mapping->no());

            if (!in_device.settings().include.empty()) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", mapping->include_name().data());
            }

            ImGui::TableNextColumn();
            ImGui::Text("%i", mapping->channel());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->data_1_as_string().data());

            if (!in_device.settings().sl_dataref.empty()) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", mapping->sl().data());
            }

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->map_text_label().data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", conversions::map_in_type_to_str(mapping->type()).data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->map_text_cmd_drf().data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->map_text_parameter().data());
        }

        ImGui::EndTable();
    }
}


/**
 * Create a table for all outbound mappings
 */
void profile_window::create_table_mapping_out(device& in_device)
{
    // Outbound mappings available for MIDI devices only
    if (in_device.type() != device_type::midi_device)
        return;

    auto& midi_dev = dynamic_cast<midi_device&>(in_device);

    if (ImGui::BeginTable("mapping_out",
                          8,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("No", ImGuiTableColumnFlags_WidthFixed, 40);
        ImGui::TableSetupColumn("Include", ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Data 1", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Sublayer", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Dataref", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        for (const auto& mapping: midi_dev.mapping_out()) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("%i", mapping->no());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->include_name().data());

            ImGui::TableNextColumn();
            ImGui::Text("%i", mapping->channel());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->data_1_as_string().data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->sl().data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", conversions::map_out_type_to_str(mapping->type()).data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->map_text_drf().data());

            ImGui::TableNextColumn();
            ImGui::Text("%s", mapping->map_text_parameter().data());
        }

        ImGui::EndTable();
    }
}

} // Namespace xmidictrl
