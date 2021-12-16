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

#include "settings.h"

// Standard
#include <filesystem>
#include <utility>

// XMidiCtrl
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
settings::settings(std::shared_ptr<xplane> xp) : config(std::move(xp))
{
    // build name for general settings file

    if (!load(get_settings_filename()))
        LOG_WARN << " --> Will use default settings" << LOG_END
}


/**
 * Destructor
 */
settings::~settings() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Set the active log level
 */
void settings::set_log_level(log_level level)
{
    LOG_ALL << "Set logging level to '" << utils::log_level_as_text(level) << "'" << LOG_END
    m_config[CFG_KEY_LOG_LEVEL] = utils::log_level_as_code(level);
}


/**
 * Return the active log level
 */
log_level settings::get_log_level()
{
    std::string logLevel = toml::find_or<std::string>(m_config, CFG_KEY_LOG_LEVEL,
                                                      utils::log_level_as_code(log_level::debug));
    return utils::log_level_from_code(logLevel);
}


/**
 * Set if midi logging is active
 */
void settings::set_log_midi(bool enabled)
{
    m_config[CFG_KEY_LOG_MIDI] = enabled;
}


/**
 * Return if midi logging is active
 */
bool settings::get_log_midi()
{
    return toml::find_or<bool>(m_config, CFG_KEY_LOG_MIDI, true);
}


/**
 * Set if the messages dialog should be displayed in case of error
 */
void settings::set_show_messages(const bool enabled)
{
    m_config[CFG_KEY_SHOW_MSG_DIALOG] = enabled;
}


/**
 * Return if the messages dialog should be displayed in case of error
 */
bool settings::get_show_messages()
{
    return toml::find_or<bool>(m_config, CFG_KEY_SHOW_MSG_DIALOG, true);
}


/**
 * Save the current settings
 */
void settings::save_settings()
{
    std::ofstream stream;
    std::string filename = get_settings_filename();

    if (filename.empty()) {
        LOG_ERROR << "Could not determine file name for general settings" << LOG_END
        return;
    }

    // check if our directory already exists in the preference folder
    if (!utils::create_preference_folders(m_xp))
        return;

    stream.open(filename, std::ios_base::out | std::ios_base::trunc);

    if (!stream.is_open()) {
        LOG_ERROR << "Could not save general settings file '" << filename << "'" << LOG_END
        return;
    }

    stream << m_config;
    stream.close();
}


/**
 * Returns the file name for the general settings
 */
std::string settings::get_settings_filename()
{
    std::string filename = m_xp->preferences_path().data() + std::string(XMIDICTRL_NAME) +
                           std::string(SETTINGS_FILE_SUFFIX);

    return filename;
}

} // Namespace xmidictrl