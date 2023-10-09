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

#include "settings_window.h"

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
settings_window::settings_window(text_logger& in_log, environment& in_env, settings& in_settings)
    : imgui_window(in_log, in_env, 1000, 540),
      m_settings(in_settings)
{
    m_debug_mode = m_settings.debug_mode();
    m_log_midi = m_settings.log_midi();
    m_show_messages = m_settings.show_messages();

    m_virtual_channel = m_settings.virtual_channel();
    m_default_outbound_delay = m_settings.default_outbound_delay();

    m_max_text_messages = m_settings.max_text_messages();
    m_max_midi_messages = m_settings.max_midi_messages();

    m_note_name = static_cast<int>(m_settings.note_name());

    m_default_text_sort = static_cast<int>(m_settings.default_text_sort());
    m_default_midi_sort = static_cast<int>(m_settings.default_midi_sort());

    m_use_common_profile = m_settings.use_common_profile();

    m_info_disabled = m_settings.info_disabled();
    m_info_position = m_settings.info_position();
    m_info_offset_x = m_settings.info_offset_x();
    m_info_offset_y = m_settings.info_offset_y();
    m_info_seconds = m_settings.info_seconds();

    m_path_xplane = in_env.xplane_path().string();
    m_path_plugin = in_env.plugin_path().string();
    m_path_preferences = in_env.preferences_path().string();
    m_path_profiles = in_env.profiles_path().string();
    m_path_includes = in_env.includes_path().string();

    set_title(std::string(XMIDICTRL_NAME) + " - Settings");
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void settings_window::create_widgets()
{
    if (ImGui::BeginTabBar("SETTINGS_TAB")) {
        create_tab_general();
        create_tab_midi();
        create_tab_logging();
        create_tab_paths();

        ImGui::EndTabBar();
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create tab for general settings
 */
void settings_window::create_tab_general()
{
    if (ImGui::BeginTabItem("General")) {
        ImGui::AlignTextToFramePadding();

        ImGui::TextUnformatted("GENERAL SETTINGS");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Checkbox("Use common aircraft profile", &m_use_common_profile);
        ImGui::Checkbox("Show errors after loading an aircraft", &m_show_messages);

        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::TextUnformatted("INFO WINDOW");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Checkbox("Disable info window", &m_info_disabled);
        ImGui::NewLine();

        ImGui::TextUnformatted("Window Position:");
        ImGui::SameLine(250);
        ImGui::PushItemWidth(200);
        if (ImGui::BeginCombo("##info_position", info_position_as_text(m_info_position).c_str())) {
            if (ImGui::Selectable(info_position_as_text(window_position::top_left).c_str(),
                                  m_info_position == window_position::top_left)) {
                m_info_position = window_position::top_left;
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::Selectable(info_position_as_text(window_position::top_center).c_str(),
                                  m_info_position == window_position::top_center)) {
                m_info_position = window_position::top_center;
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::Selectable(info_position_as_text(window_position::top_right).c_str(),
                                  m_info_position == window_position::top_right)) {
                m_info_position = window_position::top_right;
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::Selectable(info_position_as_text(window_position::bottom_left).c_str(),
                                  m_info_position == window_position::bottom_left)) {
                m_info_position = window_position::bottom_left;
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::Selectable(info_position_as_text(window_position::bottom_center).c_str(),
                                  m_info_position == window_position::bottom_center)) {
                m_info_position = window_position::bottom_center;
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::Selectable(info_position_as_text(window_position::bottom_right).c_str(),
                                  m_info_position == window_position::bottom_right)) {
                m_info_position = window_position::bottom_right;
                ImGui::SetItemDefaultFocus();
            }

            if (ImGui::Selectable(info_position_as_text(window_position::center).c_str(),
                                  m_info_position == window_position::center)) {
                m_info_position = window_position::center;
                ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::SameLine(600);
        if (ImGui::Button("  " ICON_FA_ROTATE_RIGHT "  Reset to default  ")) {
            m_info_position = window_position::bottom_left;
            m_info_offset_x = 50;
            m_info_offset_y = 50;
            m_info_seconds = 3;
        }

        ImGui::NewLine();

        ImGui::TextUnformatted("Offset X:");
        ImGui::SameLine(250);
        ImGui::PushItemWidth(150);
        ImGui::InputInt("##offsetx", &m_info_offset_x, 1, 1);

        ImGui::TextUnformatted("Offset Y:");
        ImGui::SameLine(250);
        ImGui::PushItemWidth(150);
        ImGui::InputInt("##offsety", &m_info_offset_y, 1, 1);

        ImGui::NewLine();

        ImGui::TextUnformatted("Display Time:");
        ImGui::SameLine(250);
        ImGui::PushItemWidth(150);
        if (ImGui::InputInt("Seconds##time", &m_info_seconds, 1, 1)) {
            if (m_info_seconds < 1)
                m_info_seconds = 1;
        }

        ImGui::SameLine(600);
        if (ImGui::Button("  " ICON_FA_INFO "  Test info window  ")) {
            save_settings();
            plugin::instance().show_info_message("TEST_MESSAGE", "This is a test message");
        }

        ImGui::NewLine();
        ImGui::NewLine();

        if (ImGui::Button("  " ICON_FA_FLOPPY_DISK "  Save Settings  "))
            save_settings();

        ImGui::EndTabItem();
    }
}


/**
 * Create tab for MIDI settings
 */
void settings_window::create_tab_midi()
{
    if (ImGui::BeginTabItem("MIDI")) {
        ImGui::AlignTextToFramePadding();

        ImGui::TextUnformatted("MIDI SETTINGS");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::SliderInt("Channel for virtual MIDI messages", &m_virtual_channel, 1, 16);

        ImGui::NewLine();

        ImGui::SliderFloat("Default delay for outbound MIDI messages (in s)", &m_default_outbound_delay, 0, 3, "%.1f");

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();

        if (ImGui::Button("  " ICON_FA_FLOPPY_DISK "  Save Settings  "))
            save_settings();

        ImGui::EndTabItem();
    }
}


/**
 * Create tab for logging settings
 */
void settings_window::create_tab_logging()
{
    if (ImGui::BeginTabItem("Logging")) {
        ImGui::AlignTextToFramePadding();

        ImGui::TextUnformatted("GENERAL LOGGING");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Checkbox("Debug Mode", &m_debug_mode);
        ImGui::NewLine();

        ImGui::TextUnformatted("Limit for messages:");
        ImGui::SameLine(250);
        ImGui::PushItemWidth(150);
        if (ImGui::InputInt(" Messages##text", &m_max_text_messages, 50, 100)) {
            if (m_max_text_messages < 1)
                m_max_text_messages = 1;
            else if (m_max_text_messages > 5000)
                m_max_text_messages = 5000;
        }

        ImGui::SameLine(600);
        if (ImGui::Button("  " ICON_FA_ROTATE_RIGHT "  Reset to default  ##text"))
            m_max_text_messages = 1500;

        ImGui::TextUnformatted("Default sort mode:");
        ImGui::SameLine(250);
        ImGui::RadioButton("Ascending##text", &m_default_text_sort, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Descending##text", &m_default_text_sort, 1);

        ImGui::NewLine();
        ImGui::NewLine();

        ImGui::TextUnformatted("MIDI LOGGING");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Checkbox("Log MIDI messages", &m_log_midi);
        ImGui::NewLine();

        if (m_log_midi) {
            ImGui::TextUnformatted("Limit for messages:");
            ImGui::SameLine(250);
            ImGui::PushItemWidth(150);
            if (ImGui::InputInt(" Messages##midi", &m_max_midi_messages, 50, 100)) {
                if (m_max_midi_messages < 1)
                    m_max_midi_messages = 1;
                else if (m_max_midi_messages > 5000)
                    m_max_midi_messages = 5000;
            }
        }

        ImGui::SameLine(600);
        if (ImGui::Button("  " ICON_FA_ROTATE_RIGHT "  Reset to default  ##midi"))
            m_max_midi_messages = 150;

        ImGui::TextUnformatted("Default sort mode:");
        ImGui::SameLine(250);
        ImGui::RadioButton("Ascending##midi", &m_default_midi_sort, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Descending##midi", &m_default_midi_sort, 1);

        ImGui::TextUnformatted("Note names:");
        ImGui::SameLine(250);
        ImGui::RadioButton("Sharp names (e.g. C#)", &m_note_name, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Flat names (e.g. Eb)", &m_note_name, 1);

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();

        if (ImGui::Button("  " ICON_FA_FLOPPY_DISK "  Save Settings  "))
            save_settings();

        ImGui::EndTabItem();
    }
}


/**
 * Create tab for paths
 */
void settings_window::create_tab_paths()
{
    if (ImGui::BeginTabItem("Paths")) {
        ImGui::AlignTextToFramePadding();

        ImGui::TextUnformatted("PATHS");
        ImGui::Separator();
        ImGui::NewLine();

        ImGui::TextUnformatted("X-Plane:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_xplane.c_str());

        ImGui::TextUnformatted("Plugin:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_plugin.c_str());

        ImGui::TextUnformatted("Preferences:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_preferences.c_str());

        ImGui::TextUnformatted("Profiles:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_profiles.c_str());

        ImGui::TextUnformatted("Includes:");
        ImGui::SameLine(150);
        ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_includes.c_str());

        ImGui::EndTabItem();
    }
}


/**
 * Return the info position as text
 */
std::string settings_window::info_position_as_text(window_position in_position)
{
    switch (in_position) {
        case window_position::top_left:
            return "Top - Left";

        case window_position::top_center:
            return "Top - Center";

        case window_position::top_right:
            return "Top - Right";

        case window_position::bottom_left:
            return "Bottom - Left";

        case window_position::bottom_center:
            return "Bottom - Center";

        case window_position::bottom_right:
            return "Bottom - Right";

        case window_position::center:
            return "Center";
    }

    return {};
}


/**
 * Save settings
 */
void settings_window::save_settings()
{
    m_settings.set_debug_mode(m_debug_mode);
    m_settings.set_log_midi(m_log_midi);

    m_settings.set_virtual_channel(m_virtual_channel);
    m_settings.set_default_outbound_delay(m_default_outbound_delay);

    m_settings.set_max_text_messages(m_max_text_messages);
    m_settings.set_max_midi_messages(m_max_midi_messages);

    m_settings.set_note_name(static_cast<note_name_type>(m_note_name));

    m_settings.set_default_text_sort(static_cast<sort_mode>(m_default_text_sort));
    m_settings.set_default_midi_sort(static_cast<sort_mode>(m_default_midi_sort));

    m_settings.set_show_messages(m_show_messages);

    m_settings.set_use_common_profile(m_use_common_profile);

    m_settings.set_info_disabled(m_info_disabled);
    m_settings.set_info_position(m_info_position);
    m_settings.set_info_offset_x(m_info_offset_x);
    m_settings.set_info_offset_y(m_info_offset_y);
    m_settings.set_info_seconds(m_info_seconds);

    m_settings.save_settings();
}

} // Namespace xmidictrl