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

// Standard
#include <string>

// Font Awesome
#include <IconsFontAwesome6.h>

// XMidiCtrl
#include "settings_window.h"
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
settings_window::settings_window(text_logger &in_log, xplane &in_xp, settings &in_settings)
    : imgui_window(in_log, in_xp, 1000, 600),
      m_settings(in_settings)
{
    m_debug_mode = m_settings.debug_mode();
    m_log_midi = m_settings.log_midi();
    m_show_messages = m_settings.show_messages();

    m_max_text_messages = m_settings.max_text_messages();
    m_max_midi_messages = m_settings.max_midi_messages();

    m_default_text_sort = static_cast<int>(m_settings.default_text_sort());
    m_default_midi_sort = static_cast<int>(m_settings.default_midi_sort());

    m_common_profile = m_settings.common_profile();

    m_path_xplane = in_xp.xplane_path();
    m_path_plugin = in_xp.plugin_path();
    m_path_preferences = in_xp.preferences_path();
    m_path_profiles = in_xp.profiles_path();

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
    ImGui::Text("LOGGING SETTINGS");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::AlignTextToFramePadding();

    ImGui::Checkbox("Debug Mode", &m_debug_mode);
    if (m_debug_mode) {
        ImGui::SameLine(500);
        ImGui::Text("Limit for message log:");
        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        if (ImGui::InputInt(" Messages##text", &m_max_text_messages, 50, 100)) {
            if (m_max_text_messages < 1)
                m_max_text_messages = 1;
            else if (m_max_text_messages > 5000)
                m_max_text_messages = 5000;
        }
    }
    ImGui::TextUnformatted("Default sort mode:");
    ImGui::SameLine();
    ImGui::RadioButton("Ascending##text", &m_default_text_sort, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Descending##text", &m_default_text_sort, 1);

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Checkbox("Log MIDI messages", &m_log_midi);
    if (m_log_midi) {
        ImGui::SameLine(500);
        ImGui::Text("Limit for MIDI log:");
        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        if (ImGui::InputInt(" Messages##midi", &m_max_midi_messages, 50, 100)) {
            if (m_max_midi_messages < 1)
                m_max_midi_messages = 1;
            else if (m_max_midi_messages > 5000)
                m_max_midi_messages = 5000;
        }
    }
    ImGui::TextUnformatted("Default sort mode:");
    ImGui::SameLine();
    ImGui::RadioButton("Ascending##midi", &m_default_midi_sort, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Descending##midi", &m_default_midi_sort, 1);

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("GENERAL SETTINGS");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Checkbox("Use common aircraft profile", &m_common_profile);
    ImGui::Checkbox("Show errors after loading an aircraft", &m_show_messages);

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("PATHS");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("X-Plane:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_xplane.c_str());

    ImGui::Text("Plugin:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_plugin.c_str());

    ImGui::Text("Preferences:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_preferences.c_str());

    ImGui::Text("Profiles:");
    ImGui::SameLine(150);
    ImGui::TextColored(COL_TEXT_VALUE, "%s", m_path_profiles.c_str());

    ImGui::NewLine();
    if (ImGui::Button("  " ICON_FA_FLOPPY_DISK " Save Settings  ")) {
        m_settings.set_debug_mode(m_debug_mode);
        m_settings.set_log_midi(m_log_midi);

        m_settings.set_max_text_messages(m_max_text_messages);
        m_settings.set_max_midi_messages(m_max_midi_messages);

        m_settings.set_default_text_sort(static_cast<sort_mode>(m_default_text_sort));
        m_settings.set_default_midi_sort(static_cast<sort_mode>(m_default_midi_sort));

        m_settings.set_show_messages(m_show_messages);

        m_settings.set_common_profile(m_common_profile);

        m_settings.save_settings();
    }
}

} // Namespace xmidictrl