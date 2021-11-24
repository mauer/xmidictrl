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
config::config(xplane::ptr xp)
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
        LOG_DEBUG << "File '" << filename.data() << "' loaded successfully" << LOG_END
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
    m_config = toml::value();
}

} // Namespace xmidictrl
