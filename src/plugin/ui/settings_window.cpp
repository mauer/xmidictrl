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

// Standard
#include <string>

// XMidiCtrl
#include "logger.h"
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
    m_log_level = m_settings->get_log_level();
    m_log_midi = m_settings->get_log_midi();
    m_show_messages = m_settings->get_show_messages();

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

    ImGui::AlignTextToFramePadding();
    ImGui::Text("X-Plane:");
    ImGui::SameLine(150);
    ImGui::TextColored((ImVec4) ImColor(255, 127, 39), "%s", m_path_xplane.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Plugin:");
    ImGui::SameLine(150);
    ImGui::TextColored((ImVec4) ImColor(255, 127, 39), "%s", m_path_plugin.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Preferences:");
    ImGui::SameLine(150);
    ImGui::TextColored((ImVec4) ImColor(255, 127, 39), "%s", m_path_preferences.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Profiles:");
    ImGui::SameLine(150);
    ImGui::TextColored((ImVec4) ImColor(255, 127, 39), "%s", m_path_profiles.c_str());

    ImGui::NewLine();
    if (ImGui::Button("Save Settings")) {
        m_settings->set_log_level(m_log_level);
        m_settings->set_log_midi(m_log_midi);
        m_settings->set_show_messages(m_show_messages);

        m_settings->save_settings();
    }
}

} // Namespace xmidictrl