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
settings_window::settings_window(settings::ptr settings) :
ImGuiWindow(500, 200)
{
    LOG_DEBUG << "Create the settings dialog" << LOG_END

    m_settings = std::move(settings);
    m_selectedLogLevel = m_settings->loglevel();

    setTitle(std::string(XMIDICTRL_NAME) + " Settings");
}


/**
 * Destructor
 */
settings_window::~settings_window() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void settings_window::createWidgets() {
    ImGui::Text("GENERAL");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("Logging Level:");
    ImGui::SameLine(150);

    if (ImGui::BeginCombo("", utils::get_log_level_Text(m_selectedLogLevel).c_str())) {
        if (ImGui::Selectable(utils::get_log_level_Text(log_level::error).c_str(),
                              m_selectedLogLevel == log_level::error)) {
            m_selectedLogLevel = log_level::error;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(utils::get_log_level_Text(log_level::warn).c_str(),
                              m_selectedLogLevel == log_level::warn)) {
            m_selectedLogLevel = log_level::warn;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(utils::get_log_level_Text(log_level::info).c_str(),
                              m_selectedLogLevel == log_level::info)) {
            m_selectedLogLevel = log_level::info;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(utils::get_log_level_Text(log_level::debug).c_str(),
                              m_selectedLogLevel == log_level::debug)) {
            m_selectedLogLevel = log_level::debug;
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Show Messages Dialog in case of errors", &m_showMessagesDialog);

    ImGui::NewLine();
    ImGui::NewLine();

    if (ImGui::Button("Save Settings")) {
        m_settings->set_loglevel(m_selectedLogLevel);
        m_settings->set_show_messages(m_showMessagesDialog);
        hide();
    }
}

} // Namespace XMidiCtrl