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

#include "config.h"

// Standard
#include <filesystem>

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
config::config(environment& in_env)
    : m_env(in_env)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the environment
 */
environment& config::env() const
{
    return m_env;
}


/**
 * Load config from a file
 */
bool config::load_file(text_logger& in_log, std::string_view in_filename)
{
    // just in case...
    close_file(in_log);

    if (toml_utils::load_file(in_log, in_filename, m_config))
        m_filename = in_filename;
    else
        return false;

    return true;
}


/**
 * Close the current config
 */
void config::close_file(text_logger& in_log)
{
    if (!m_filename.empty())
        in_log.debug("File '" + m_filename + "' closed");

    m_config = toml::value();
    m_filename.clear();
}

} // Namespace xmidictrl
