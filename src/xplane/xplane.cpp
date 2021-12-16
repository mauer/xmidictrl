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

#include "xplane.h"

// X-Plane SDK
#include "XPLMPlanes.h"
#include "XPLMPlugin.h"

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
xplane::xplane()
{
    // access to X-Plane commands
    m_commands = std::make_unique<commands>();

    // access to X-Plane data
    m_datarefs = std::make_unique<data>();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the plugin ID
 */
XPLMPluginID xplane::plugin_id()
{
    if (m_plugin_id == -1) {
        m_plugin_id = XPLMGetMyID();
    }

    return m_plugin_id;
}


/**
 * Return the path of the plugin installation
 */
std::string_view xplane::plugin_path()
{
    if (m_plugin_path.empty()) {
        char path[512];
        XPLMGetPluginInfo(plugin_id(), nullptr, path, nullptr, nullptr);

        // remove file name
        XPLMExtractFileAndPath(path);

        m_plugin_path = std::string(path) + XPLMGetDirectorySeparator();

        LOG_DEBUG << "Plugin Path = '" << m_plugin_path << "'" << LOG_END
    }

    return m_plugin_path;
}


/**
 * Return the path of the X-Plane installation
 */
std::string_view xplane::xplane_path()
{
    if (m_xplane_path.empty()) {
        char path[512];
        XPLMGetSystemPath(path);

        m_xplane_path = std::string(path);

        LOG_DEBUG << "X-Plane Path = '" << m_xplane_path << "'" << LOG_END
    }

    return m_xplane_path;
}


/**
 * Return the preferences path of the plugin
 */
std::string_view xplane::preferences_path()
{
    if (m_preferences_path.empty()) {
        char path[512];
        XPLMGetPrefsPath(path);

        // remove file name
        XPLMExtractFileAndPath(path);

        m_preferences_path = std::string(path) + XPLMGetDirectorySeparator() + XMIDICTRL_NAME
                             + XPLMGetDirectorySeparator();

        LOG_DEBUG << "Preferences Path = '" << m_preferences_path << "'" << LOG_END
    }

    return m_preferences_path;
}


/**
 * Return the profiles path within the preferences
 */
std::string_view xplane::profiles_path()
{
    if (m_profiles_path.empty()) {
        m_profiles_path = std::string(preferences_path()) + PROFILES_DIRECTORY_NAME + "/";

        LOG_DEBUG << "Profiles Path = '" << m_profiles_path << "'" << LOG_END
    }

    return m_profiles_path;
}


/**
 * Return the current aircraft path
 */
std::string xplane::current_aircraft_path()
{
    char aircraft_filename[256];
    char aircraft_path[512];

    // aircraft with index 0 is the user aircraft
    XPLMGetNthAircraftModel(0, aircraft_filename, aircraft_path);

    if (!std::string(aircraft_path).empty())
        XPLMExtractFileAndPath(aircraft_path);

    return std::string(aircraft_path) + "/";
}


/**
 * Return the author of the current aircraft
 */
std::string xplane::current_aircraft_author()
{
    std::string value;

    if (m_datarefs->read("sim/aircraft/view/acf_author", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the ICAO of the current aircraft
 */
std::string xplane::current_aircraft_icao()
{
    std::string value;

    if (m_datarefs->read("sim/aircraft/view/acf_ICAO", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the description of the current aircraft
 */
std::string xplane::current_aircraft_descr()
{
    std::string value;

    if (m_datarefs->read("sim/aircraft/view/acf_descrip", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the cmd object
 */
commands &xplane::cmd()
{
    return *m_commands;
}


/**
 * Return the dataref object
 */
data &xplane::datarefs()
{
    return *m_datarefs;
}

} // Namespace XMidiCtrl