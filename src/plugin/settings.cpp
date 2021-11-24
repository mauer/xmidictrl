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
settings::settings(xplane::ptr xplane)
    : config(xplane)
{
    // build name for general settings file
    m_filename = m_xp->preferences_path().data() + std::string(XMIDICTRL_NAME) + std::string(SETTINGS_FILE_SUFFIX);

    if (!load(m_filename))
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
void settings::set_loglevel(log_level log_level)
{
    LOG_ALL << "Set log level to '" << utils::get_log_level_Text(log_level) << "'" << LOG_END

    // set log level in logging instance
    logger::instance().set_log_level(log_level);

    // store it in the general settings
    m_config[CFG_KEY_LOG_LEVEL] = utils::get_log_level_code(log_level);

    // save settings
    save_settings();
}


/**
 * Return the active log level
 */
log_level settings::loglevel()
{
    std::string logLevel = toml::find_or<std::string>(m_config, CFG_KEY_LOG_LEVEL,
                                                      utils::get_log_level_code(log_level::debug));
    return utils::get_log_level_from_code(logLevel);
}


/**
 * Set if the messages dialog should be displayed in case of error
 */
void settings::set_show_messages(const bool show_messages)
{
    // store it in the general settings
    m_config[CFG_KEY_SHOW_MSG_DIALOG] = show_messages;

    // save settings
    save_settings();
}


/**
 * Return if the messages dialog should be displayed in case of error
 */
bool settings::show_messages()
{
    return toml::find_or<bool>(m_config, CFG_KEY_SHOW_MSG_DIALOG, true);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Save the current settings
 */
void settings::save_settings()
{
    std::ofstream stream;

    if (m_filename.empty()) {
        LOG_ERROR << "Could not determine file name for general settings" << LOG_END
        return;
    }

    // check if our directory already exists in the preference folder
    if (!std::filesystem::exists(m_xp->preferences_path())) {
        LOG_INFO << "Directory '" << m_xp->preferences_path() << "' not found" << LOG_END

        if (std::filesystem::create_directory(m_xp->preferences_path())) {
            LOG_INFO << "Directory '" << m_xp->preferences_path() << "' created" << LOG_END
        } else {
            LOG_ERROR << "Could not create directory '" << m_xp->preferences_path() << "'" << LOG_END
            return;
        }
    }

    stream.open(m_filename, std::ios_base::out | std::ios_base::trunc);

    if (!stream.is_open()) {
        LOG_ERROR << "Could not save general settings file '" << m_filename << "'" << LOG_END
        return;
    }

    stream << m_config;
    stream.close();
}

} // Namespace XMidiCtrl