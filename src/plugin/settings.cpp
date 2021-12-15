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
    std::string
        logLevel = toml::find_or<std::string>(m_config, CFG_KEY_LOG_LEVEL, utils::log_level_as_code(log_level::debug));
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
    if (m_config[CFG_KEY_SHOW_MSG_DIALOG].as_boolean() != enabled) {
        // store it in the general settings
        m_config[CFG_KEY_SHOW_MSG_DIALOG] = enabled;

        // save settings
        save_settings();
    }
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