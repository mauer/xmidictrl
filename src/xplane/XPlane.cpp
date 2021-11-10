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
#include "XPLMPlanes.h"
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
    m_pluginId   = -1;
    m_pluginPath = "";
    m_xplanePath = "";

    // access to X-Plane commands
    m_commands = std::make_shared<Commands>();

    // access to X-Plane data
    m_data = std::make_shared<Data>();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the plugin ID
 */
XPLMPluginID XPlane::pluginId() {
    if (m_pluginId == -1) {
        m_pluginId = XPLMGetMyID();
        LOG_DEBUG << "X-Plane Plugin ID = '" << m_pluginId << "'" << LOG_END
    }

    return m_pluginId;
}


/**
 * Return the path of the plugin installation
 */
std::string_view XPlane::pluginPath() {
    if (m_pluginPath.empty()) {
        char path[512];
        XPLMGetPluginInfo(pluginId(), nullptr, path, nullptr, nullptr);

        m_pluginPath = std::string(path);
        LOG_DEBUG << XMIDICTRL_NAME << " Path = '" << m_pluginPath << "'" << LOG_END
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
        LOG_DEBUG << "X-Plane Path = '" << m_xplanePath << "'" << LOG_END
    }

    return m_xplanePath;
}


/**
 * Return the preferences path of the plugin
 */
std::string_view XPlane::preferencesPath() {
    if (m_preferencesPath.empty()) {
        char path[512];
        XPLMGetPrefsPath(path);

        // remove file name
        XPLMExtractFileAndPath(path);

        m_preferencesPath = std::string(path) + "/" + XMIDICTRL_NAME + "/";
        LOG_DEBUG << "Preferences Path = '" << m_preferencesPath << "'" << LOG_END
    }

    return m_preferencesPath;
}


/**
 * Return the profiles path within the preferences
 */
std::string_view XPlane::profilesPath() {
    if (m_profilesPath.empty()) {
        m_profilesPath = std::string(preferencesPath()) + PROFILES_DIRECTORY_NAME + "/";
        LOG_DEBUG << "Profiles Path = '" << m_profilesPath << "'" << LOG_END
    }

    return m_profilesPath;
}


/**
 * Return the current aircraft path
 */
std::string XPlane::currentAircraftPath() {
    char aircraftFileName[256];
    char aircraftPath[512];

    // aircraft with index 0 is the user aircraft
    XPLMGetNthAircraftModel(0, aircraftFileName, aircraftPath);

    if (!std::string(aircraftPath).empty())
        XPLMExtractFileAndPath(aircraftPath);

    LOG_DEBUG << "Current Aircraft Path = " << aircraftPath << LOG_END

    return std::string(aircraftPath) + "/";
}


/**
 * Return the author of the current aircraft
 */
std::string XPlane::currentAircraftAuthor() {
    return m_data->readByte("sim/aircraft/view/acf_author");
}


/**
 * Return the ICAO of the current aircraft
 */
std::string XPlane::currentAircraftICAO() {
    return m_data->readByte("sim/aircraft/view/acf_ICAO");
}


/**
 * Return the description of the current aircraft
 */
std::string XPlane::currentAircraftDescription() {
    return m_data->readByte("sim/aircraft/view/acf_descrip");
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