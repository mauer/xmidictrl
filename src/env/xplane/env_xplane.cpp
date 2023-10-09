//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#include "env_xplane.h"

// X-Plane SDK
#include "XPLMPlanes.h"
#include "XPLMPlugin.h"
#include "XPLMUtilities.h"

// XMidiCtrl
#include "text_logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
env_xplane::env_xplane(text_logger& in_log)
    : environment(in_log)
{
    // access to X-Plane commands
    m_cmd = std::make_unique<cmd_xplane>();

    // access to X-Plane data
    m_drf = std::make_unique<data_xplane>();
}


/**
 * Destructor
 */
env_xplane::~env_xplane()
{
    m_cmd.reset();
    m_drf.reset();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the path of the X-Plane installation
 */
std::filesystem::path env_xplane::xplane_path()
{
    if (m_xplane_path.empty()) {
        char path[512];
        XPLMGetSystemPath(path);

        m_xplane_path.assign(path);
    }

    return m_xplane_path;
}


/**
 * Return the path of the plugin directory
 */
std::filesystem::path env_xplane::plugin_path()
{
    if (m_plugin_path.empty()) {
        char path[512];
        XPLMGetPluginInfo(m_plugin_id, nullptr, path, nullptr, nullptr);
        m_plugin_path.assign(path);

        log().debug("Plugin path = " + std::string(path));

        // remove file name
        m_plugin_path.assign(m_plugin_path.remove_filename());
        log().debug("Plugin path (after filename removed) = " + m_plugin_path.string());

        // remove subdirectory
        m_plugin_path.assign(m_plugin_path.parent_path());
        log().debug("Plugin path (after subdirectory removed) = " + m_plugin_path.string());
    }

    return m_plugin_path;
}


/**
 * Return the preferences path of the plugin
 */
std::filesystem::path env_xplane::preferences_path()
{
    if (m_preferences_path.empty()) {
        char path[512];
        XPLMGetPrefsPath(path);

        // remove file name
        XPLMExtractFileAndPath(path);

        m_preferences_path = std::string(path) + XPLMGetDirectorySeparator() + XMIDICTRL_NAME
                             + XPLMGetDirectorySeparator();
    }

    return m_preferences_path;
}


/**
 * Return the profiles path within the preferences
 */
std::filesystem::path env_xplane::profiles_path()
{
    if (m_profiles_path.empty())
        m_profiles_path = preferences_path().string() + PROFILES_DIRECTORY_NAME + "/";

    return m_profiles_path;
}


/**
 * Return the includes path within the preferences
 */
std::filesystem::path env_xplane::includes_path()
{
    if (m_profiles_path.empty())
        m_profiles_path = preferences_path().string() + INCLUDES_DIRECTORY_NAME + "/";

    return m_profiles_path;
}


/**
 * Return the current aircraft path
 */
std::filesystem::path env_xplane::current_aircraft_path()
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
std::string env_xplane::current_aircraft_author()
{
    std::string value;

    if (m_drf->read(log(), "sim/aircraft/view/acf_author", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the ICAO of the current aircraft
 */
std::string env_xplane::current_aircraft_icao()
{
    std::string value;

    if (m_drf->read(log(), "sim/aircraft/view/acf_ICAO", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the acf name of the current aircraft
 */
std::string env_xplane::current_aircraft_acf_name()
{
    char aircraft_filename[256];
    char aircraft_path[512];

    // aircraft with index 0 is the user aircraft
    XPLMGetNthAircraftModel(0, aircraft_filename, aircraft_path);
    std::filesystem::path file = aircraft_filename;

    return file.replace_extension().string();
}


/**
 * Return the description of the current aircraft
 */
std::string env_xplane::current_aircraft_descr()
{
    std::string value;

    if (m_drf->read(log(), "sim/aircraft/view/acf_descrip", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the cmd object
 */
commands& env_xplane::cmd()
{
    return *m_cmd;
}


/**
 * Return the dataref object
 */
data& env_xplane::drf()
{
    return *m_drf;
}

} // Namespace xmidictrl