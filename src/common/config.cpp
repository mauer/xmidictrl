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

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
config::config(std::shared_ptr<xplane> xp)
    : m_xp(std::move(xp))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Load config from a file
 */
bool config::load(std::string_view filename)
{
    // just in case...
    close();

    // check file name
    if (filename.empty()) {
        LOG_ERROR << "Cannot load file, because the given filename is empty" << LOG_END
        return false;
    }

    // check if file exists
    if (!std::filesystem::exists(filename)) {
        LOG_ERROR << "File '" << filename.data() << "' not found" << LOG_END
        return false;
    }

    try {
        // load config file
        m_config = toml::parse(filename.data());
        m_filename = filename;
        
        LOG_DEBUG << "File '" << m_filename << "' loaded successfully" << LOG_END
    } catch (const toml::syntax_error &error) {
        LOG_ERROR << "Error parsing file '" << filename.data() << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
        return false;

    } catch (const std::runtime_error &error) {
        LOG_ERROR << "Error opening file '" << filename.data() << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
        return false;
    }

    return true;
}


/**
 * Close the current config
 */
void config::close()
{
    if (!m_filename.empty())
        LOG_DEBUG << "File '" << m_filename << "' closed" << LOG_END

    m_config = toml::value();
    m_filename.clear();
}

} // Namespace xmidictrl
