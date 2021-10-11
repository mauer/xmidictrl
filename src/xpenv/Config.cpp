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

// X-Plane Environment
#include "Config.h"
#include "PluginLogger.h"

namespace XPEnv {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Config::Config() {
    clear();
}


/**
 * Destructor
 */
Config::~Config() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Load config from a file
 */
bool Config::load(std::string_view fileName) {
    clear();

    // check file name
    if (fileName.empty()) {
        LOG_ERROR << "CONFIG :: Cannot load config file, because the given filename is empty" << LOG_END
        return false;
    }

    // check if file exists
    if (!std::filesystem::exists(fileName))
    {
        LOG_INFO << "CONFIG :: Config file '" << fileName.data() << "' not found" << LOG_END
        return false;
    }

    try {
        // load config
        m_config = toml::parse(fileName);
        LOG_INFO << "CONFIG :: Config file '" << fileName.data() << "' loaded successfully" << LOG_END
    } catch (const toml::syntax_error& error) {
        LOG_ERROR << "CONFIG :: Error parsing config file '" << fileName.data() << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
        return false;

    } catch (const std::runtime_error& error) {
        LOG_ERROR << "PROFILE :: Error opening config file '" << fileName.data() << "'" << LOG_END
        LOG_ERROR << error.what() << LOG_END
        return false;
    }

    return true;
}


/**
 * Clear the config
 */
void Config::clear() {
    m_config = toml::value();
}

} // XPEnv
