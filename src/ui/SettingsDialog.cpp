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
#include "Config.h"
#include "SettingsDialog.h"

using namespace XPEnv;

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
SettingsDialog::SettingsDialog() :
ImGuiWindow(500, 200)
{
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
    const char* logLevelItems[] = { "Debug", "Info", "Warning", "Error" };
    ImGui::Text("Logging Level:");
    ImGui::SameLine();
    ImGui::Combo("", &m_loggingLevel, logLevelItems, IM_ARRAYSIZE(logLevelItems));

    if (ImGui::Button("Save")) {


        hide();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
        hide();
}

} // Namespace XMidiCtrl