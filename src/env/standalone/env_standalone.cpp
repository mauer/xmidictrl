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

#include "env_standalone.h"

// Standard
#include <filesystem>

// XMidiCtrl
#include "cmd_standalone.h"
#include "data_standalone.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
env_standalone::env_standalone(text_logger &in_log)
    : environment(in_log)
{
    // access to virtual commands
    m_cmd = std::make_unique<cmd_standalone>();

    // access to virtual data
    m_drf = std::make_unique<data_standalone>();
}


/**
 * Destructor
 */
env_standalone::~env_standalone()
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
std::filesystem::path env_standalone::xplane_path()
{
    return {};
}


/**
 * Return the path of the plugin directory
 */
std::filesystem::path env_standalone::plugin_path()
{
    return {};
}


/**
 * Return the preferences path of the plugin
 */
std::filesystem::path env_standalone::preferences_path()
{
    return {};
}


/**
 * Return the profiles path within the preferences
 */
std::filesystem::path env_standalone::profiles_path()
{
    return {};
}


/**
 * Return the includes path within the preferences
 */
std::filesystem::path env_standalone::includes_path()
{
    return {};
}


/**
 * Return the current aircraft path
 */
std::filesystem::path env_standalone::current_aircraft_path()
{
    return {};
}


/**
 * Return the author of the current aircraft
 */
std::string env_standalone::current_aircraft_author()
{
    return {};
}


/**
 * Return the ICAO of the current aircraft
 */
std::string env_standalone::current_aircraft_icao()
{
    return {};
}


/**
 * Return the acf name of the current aircraft
 */
std::string env_standalone::current_aircraft_acf_name()
{
    return {};
}


/**
 * Return the description of the current aircraft
 */
std::string env_standalone::current_aircraft_descr()
{
    return {};
}


/**
 * Return the cmd object
 */
commands &env_standalone::cmd()
{
    return *m_cmd;
}


/**
 * Return the dataref object
 */
data &env_standalone::drf()
{
    return *m_drf;
}


} // Namespace xmidictrl