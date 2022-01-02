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

// XMidiCtrl
#include "settings_window.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
settings_window::settings_window(const std::shared_ptr<xplane>& xp, std::shared_ptr<settings> set)
    : ImGuiWindow(xp, 1000, 450),
      m_settings(std::move(set))
{
    m_log_level = m_settings->logging_level();
    m_log_midi = m_settings->log_midi();
    m_show_messages = m_settings->show_messages();

    m_path_xplane = xp->xplane_path();
    m_path_plugin = xp->plugin_path();
    m_path_preferences = xp->preferences_path();
    m_path_profiles = xp->profiles_path();

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
    ImGui::Text("Logging Level:");
    ImGui::SameLine(150);

    ImGui::PushItemWidth(200);
    if (ImGui::BeginCombo("", utils::log_level_as_text(m_log_level).c_str())) {
        if (ImGui::Selectable(utils::log_level_as_text(log_level::error).c_str(),
                              m_log_level == log_level::error)) {
            m_log_level = log_level::error;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(utils::log_level_as_text(log_level::warn).c_str(),
                              m_log_level == log_level::warn)) {
            m_log_level = log_level::warn;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(utils::log_level_as_text(log_level::info).c_str(),
                              m_log_level == log_level::info)) {
            m_log_level = log_level::info;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(utils::log_level_as_text(log_level::debug).c_str(),
                              m_log_level == log_level::debug)) {
            m_log_level = log_level::debug;
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::NewLine();

    ImGui::Checkbox("Log inbound and outbound MIDI messages", &m_log_midi);
    ImGui::Checkbox("Show message dialog in case of errors", &m_show_messages);

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
    if (ImGui::Button("Save Settings")) {
        m_settings->set_logging_level(m_log_level);
        m_settings->set_log_midi(m_log_midi);
        m_settings->set_show_messages(m_show_messages);

        m_settings->save_settings();
    }
}

} // Namespace xmidictrl