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

// Standard
#include <filesystem>

// XMidiCtrl
#include "Logger.h"
#include "Settings.h"
#include "Utils.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Settings::Settings(XPlane::ptr xplane)
        : Config(xplane, "Settings") {
    // build name for general settings file
    m_fileName = m_xplane->preferencesPath().data() + std::string(XMIDICTRL_NAME) + std::string(SETTINGS_FILE_SUFFIX);

    if (!load(m_fileName))
        LOG_WARN << "--> Will use default settings" << LOG_END
}


/**
 * Destructor
 */
Settings::~Settings() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Set the active log level
 */
void Settings::setLogLevel(const LogLevel logLevel) {
    LOG_ALL << "Set log level to '" << Utils::getLogLevelText(logLevel) << "'" << LOG_END

    // set log level in logging instance
    Logger::Instance().setLogLevel(logLevel);

    // store it in the general settings
    m_config[CFG_KEY_LOG_LEVEL] = Utils::getLogLevelCode(logLevel);

    // save settings
    saveSettings();
}


/**
 * Return the active log level
 */
LogLevel Settings::logLevel() {
    std::string logLevel = toml::find_or<std::string>(m_config, CFG_KEY_LOG_LEVEL, Utils::getLogLevelCode(LogLevel::Debug));
    return Utils::getLogLevelFromCode(logLevel);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Save the current settings
 */
void Settings::saveSettings() {
    std::ofstream stream;

    if (m_fileName.empty()) {
        LOG_ERROR << "Could not determine file name for general settings" << LOG_END
        return;
    }

    // check if our directory already exists in the preference folder
    if (!std::filesystem::exists(m_xplane->preferencesPath())) {
        LOG_INFO << "Directory '" << m_xplane->preferencesPath() << "' not found" << LOG_END

        if (std::filesystem::create_directory(m_xplane->preferencesPath())) {
            LOG_INFO << "Directory '" << m_xplane->preferencesPath() << "' created" << LOG_END
        } else {
            LOG_ERROR << "Could not create directory '" << m_xplane->preferencesPath() << "'" << LOG_END
            return;
        }
    }

    stream.open(m_fileName, std::ios_base::out | std::ios_base::trunc);

    if (!stream.is_open()) {
        LOG_ERROR << "Could not save general settings file '" << m_fileName << "'" << LOG_END
        return;
    }

    stream << m_config;
    stream.close();
}

} // Namespace XMidiCtrl