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
#include <filesystem>

// X-Plane SDK
#include "XPLMPlanes.h"
#include "XPLMPlugin.h"

#include "xplane.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
xplane::xplane(text_logger &in_log)
    : m_log(in_log)
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
std::string xplane::plugin_path()
{
    if (m_plugin_path.empty()) {
        char path[512];
        XPLMGetPluginInfo(plugin_id(), nullptr, path, nullptr, nullptr);

        // remove file name
        XPLMExtractFileAndPath(path);
        m_plugin_path.assign(path);

        // remove subdirectory
        std::string::size_type pos = m_plugin_path.rfind(XPLMGetDirectorySeparator());
        m_plugin_path.erase(pos + 1);
    }

    return m_plugin_path;
}


/**
 * Return the path of the X-Plane installation
 */
std::string xplane::xplane_path()
{
    if (m_xplane_path.empty()) {
        char path[512];
        XPLMGetSystemPath(path);

        m_xplane_path = std::string(path);

        m_log.debug("X-Plane Path = '" + m_xplane_path + "'");
    }

    return m_xplane_path;
}


/**
 * Return the preferences path of the plugin
 */
std::string xplane::preferences_path()
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
std::string xplane::profiles_path()
{
    if (m_profiles_path.empty())
        m_profiles_path = std::string(preferences_path()) + PROFILES_DIRECTORY_NAME + "/";

    return m_profiles_path;
}

bool xplane::create_preference_folders(text_logger& in_log)
{
    // check preference folder
    if (!std::filesystem::exists(preferences_path())) {
        in_log.info("Directory '" + preferences_path() + "' not found");

        if (std::filesystem::create_directory(preferences_path())) {
            in_log.info("Directory '" + preferences_path() + "' created");
        }
        else {
            in_log.error("Could not create directory '" + preferences_path() + "'");
            return false;
        }
    }

    // check profiles folder
    if (!std::filesystem::exists(profiles_path())) {
        in_log.info("Directory '" + profiles_path() + "' not found");

        if (std::filesystem::create_directory(profiles_path())) {
            in_log.info("Directory '" + profiles_path() + "' created");
        }
        else {
            in_log.error("Could not create directory '" + profiles_path() + "'");
            return false;
        }
    }

    return true;
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

    if (m_datarefs->read(m_log, "sim/aircraft/view/acf_author", value))
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

    if (m_datarefs->read(m_log, "sim/aircraft/view/acf_ICAO", value))
        return value;
    else {
        value.clear();
        return value;
    }
}


/**
 * Return the acf name of the current aircraft
 */
std::string xplane::current_aircraft_acf_name()
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
std::string xplane::current_aircraft_descr()
{
    std::string value;

    if (m_datarefs->read(m_log, "sim/aircraft/view/acf_descrip", value))
        return value;
    else {
        value.clear();
        return value;
    }
}

/**
 * Return the filename for the aircraft path
 */
std::string xplane::get_filename_aircraft_path(filename_prefix in_prefix)
{
    switch (in_prefix) {
    case filename_prefix::icao:
        return current_aircraft_path() + current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

    case filename_prefix::acf_name:
        return current_aircraft_path() + current_aircraft_acf_name() + "_"
            + std::string(FILENAME_PROFILE);

    default:
        return current_aircraft_path() + std::string(FILENAME_PROFILE);
    }
}


/**
 * Return the filename for the profile path
 */
std::string xplane::get_filename_profiles_path(filename_prefix in_prefix)
{
    switch (in_prefix) {
    case filename_prefix::icao:
        return profiles_path() + current_aircraft_icao() + "_" + std::string(FILENAME_PROFILE);

    case filename_prefix::acf_name:
        return profiles_path() + current_aircraft_acf_name() + "_" + std::string(FILENAME_PROFILE);

    default:
        return profiles_path() + std::string(FILENAME_PROFILE);
    }
}




/**
 * Return the cmd object
 */
simcmd_interface &xplane::cmd()
{
    return *m_commands;
}


/**
 * Return the dataref object
 */
simvar_access &xplane::datarefs()
{
    return *m_datarefs;
}

} // Namespace XMidiCtrl