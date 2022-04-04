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

#include "config.h"

// Standard
#include <filesystem>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
config::config(xplane &in_xp)
    : m_xp(in_xp)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the xplane framework
 */
xplane &config::xp() const
{
    return m_xp;
}


/**
 * Load config from a file
 */
bool config::load_file(text_logger &in_log, std::string_view in_filename)
{
    // just in case...
    close_file(in_log);

    // check file name
    if (in_filename.empty()) {
        in_log.error("Cannot load file, because the given filename is empty");
        return false;
    }

    // check if file exists
    if (!std::filesystem::exists(in_filename)) {
        in_log.error("File '%s' not found", in_filename.data());
        return false;
    }

    try {
        // load config file
        m_config = toml::parse(in_filename.data());
        m_filename = in_filename;

        in_log.debug("File '%s' loaded successfully", m_filename.data());
    } catch (const toml::syntax_error &error) {
        in_log.error("Error parsing file '%s'", in_filename.data());
        in_log.error(error.what());
        return false;

    } catch (const std::runtime_error &error) {
        in_log.error("Error opening file '%s'", in_filename.data());
        in_log.error(error.what());
        return false;
    }

    return true;
}


/**
 * Close the current config
 */
void config::close_file(text_logger &in_log)
{
    if (!m_filename.empty())
        in_log.debug("File '%s' closed", m_filename.data());

    m_config = toml::value();
    m_filename.clear();
}

} // Namespace xmidictrl
