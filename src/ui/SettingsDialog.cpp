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
#include "Logger.h"
#include "SettingsDialog.h"
#include "Utils.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
SettingsDialog::SettingsDialog(Settings::ptr settings) :
ImGuiWindow(500, 200)
{
    LOG_DEBUG << "Create the settings dialog" << LOG_END

    m_settings = std::move(settings);
    m_selectedLogLevel = m_settings->logLevel();

    setTitle(std::string(XMIDICTRL_NAME) + " Settings");
}


/**
 * Destructor
 */
SettingsDialog::~SettingsDialog() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void SettingsDialog::createWidgets() {
    ImGui::Text("GENERAL");
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("Logging Level:");
    ImGui::SameLine(150);

    if (ImGui::BeginCombo("", Utils::getLogLevelText(m_selectedLogLevel).c_str())) {
        if (ImGui::Selectable(Utils::getLogLevelText(LogLevel::Error).c_str(),
                              m_selectedLogLevel == LogLevel::Error)) {
            m_selectedLogLevel = LogLevel::Error;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(Utils::getLogLevelText(LogLevel::Warn).c_str(),
                              m_selectedLogLevel == LogLevel::Warn)) {
            m_selectedLogLevel = LogLevel::Warn;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(Utils::getLogLevelText(LogLevel::Info).c_str(),
                            m_selectedLogLevel == LogLevel::Info)) {
            m_selectedLogLevel = LogLevel::Info;
            ImGui::SetItemDefaultFocus();
        }

        if (ImGui::Selectable(Utils::getLogLevelText(LogLevel::Debug).c_str(),
                            m_selectedLogLevel == LogLevel::Debug)) {
            m_selectedLogLevel = LogLevel::Debug;
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Show Messages Dialog in case of errors", &m_showMessagesDialog);

    ImGui::NewLine();
    ImGui::NewLine();

    if (ImGui::Button("Save Settings")) {
        m_settings->setLogLevel(m_selectedLogLevel);
        m_settings->setShowMessagesDialog(m_showMessagesDialog);
        hide();
    }
}

} // Namespace XMidiCtrl