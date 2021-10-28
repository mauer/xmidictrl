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

// X-Plane SDK
#include "XPLMPlugin.h"

// XMidiCtrl
#include "Logger.h"
#include "XPlane.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
XPlane::XPlane() {
    m_pluginId = -1;
    m_pluginPath = "";
    m_xplanePath = "";

    // access to X-Plane commands
    m_commands = std::make_shared<Commands>();

    // access to X-Plane datarefs
    m_data = std::make_shared<Data>();
}


/**
 * Destructor
 */
XPlane::~XPlane() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the plugin ID
 */
XPLMPluginID XPlane::pluginId() {
    if (m_pluginId == -1)
        m_pluginId = XPLMGetMyID();

    return m_pluginId;
}


/**
 * Return the path of the plugin installation
 */
std::string_view XPlane::pluginPath() {
    if (m_xplanePath.empty()) {
        char path[512];
        XPLMGetPluginInfo(pluginId(), nullptr, path, nullptr, nullptr);

        m_pluginPath = std::string(path);
    }

    return m_pluginPath;
}


/**
 * Return the path of the X-Plane installation
 */
std::string_view XPlane::xplanePath() {
    if (m_xplanePath.empty()) {
        char path[512];
        XPLMGetSystemPath(path);

        m_xplanePath = std::string(path);
    }

    return m_xplanePath;
}


/**
 * Return the commands object
 */
Commands::ptr XPlane::commands() {
    return m_commands;
}


/**
 * Return the dataref object
 */
Data::ptr XPlane::data() {
    return m_data;
}

} // Namespace XMidiCtrl