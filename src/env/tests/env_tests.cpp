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

#include "env_tests.h"

// Standard
#include <filesystem>

// XMidiCtrl
#include "command_tests.h"
#include "data_tests.h"
#include "text_logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
env_tests::env_tests(text_logger& in_log) : environment(in_log)
{
    // access to virtual commands
    m_cmd = std::make_unique<command_tests>();

    // access to virtual data
    m_drf = std::make_unique<data_tests>();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the path of the X-Plane installation
 */
std::filesystem::path env_tests::xplane_path()
{
    return {};
}


/**
 * Return the path of the plugin directory
 */
std::filesystem::path env_tests::plugin_path()
{
    return {};
}


/**
 * Return the preferences path of the plugin
 */
std::filesystem::path env_tests::preferences_path()
{
    return {};
}


/**
 * Return the profiles path within the preferences
 */
std::filesystem::path env_tests::profiles_path()
{
    return {};
}


/**
 * Return the includes path within the preferences
 */
std::filesystem::path env_tests::includes_path()
{
    return {};
}


/**
 * Return the current aircraft path
 */
std::filesystem::path env_tests::current_aircraft_path()
{
    return {};
}


/**
 * Return the author of the current aircraft
 */
std::string env_tests::current_aircraft_author()
{
    return {};
}


/**
 * Return the ICAO of the current aircraft
 */
std::string env_tests::current_aircraft_icao()
{
    return {};
}


/**
 * Return the acf name of the current aircraft
 */
std::string env_tests::current_aircraft_acf_name()
{
    return {};
}


/**
 * Return the description of the current aircraft
 */
std::string env_tests::current_aircraft_descr()
{
    return {};
}


/**
 * Return the cmd object
 */
commands& env_tests::cmd()
{
    return *m_cmd;
}


/**
 * Return the dataref object
 */
data& env_tests::drf()
{
    return *m_drf;
}


/**
 * Return the test commands
 */
command_tests& env_tests::cmd_tests()
{
    return *m_cmd;
}

} // Namespace xmidictrl