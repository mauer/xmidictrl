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

#include "map.h"

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map::map(std::shared_ptr<xplane> xplane) : m_xp(std::move(xplane))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map::type()
{
    return map_type::none;
};


/**
 * Return the chanel number
 */
int map::ch() const
{
    return m_ch;
}


/**
 * Return the control change number
 */
int map::cc() const
{
    return m_cc;
}


/**
 * Read the config
 */
void map::read_config(toml::value &data)
{
    read_ch(data);
    read_cc(data);
}


/**
 * Check the mapping
 */
bool map::check()
{
    if (m_ch >= 1 && m_cc >= 0)
        return true;
    else
        return false;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter ch
 */
void map::read_ch(toml::value &data)
{
    m_ch = 11; // default channel 11

    try {
        // read channel
        if (data.contains(CFG_KEY_CH)) {
            m_ch = static_cast<int>( data[CFG_KEY_CH].as_integer());

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CH << "' = '" << m_ch
                      << "'" << LOG_END
        } else {
            LOG_INFO << " --> Line " << data.location().line() << " Parameter '" << CFG_KEY_CH << "' is missing, "
                     << "will use default channel '11'" << LOG_END

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CH << "' = '" << m_ch
                      << "' (Default Value)" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << " --> Line " << data.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}


/**
 * Read parameter cc
 */
void map::read_cc(toml::value &data)
{
    m_cc = -1;

    try {
        // read control change
        if (data.contains(CFG_KEY_CC)) {
            m_cc = static_cast<int>( data[CFG_KEY_CC].as_integer());

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CC << "' = '" << m_cc
                      << "'" << LOG_END
        } else if (data.contains(CFG_KEY_CC_DEPRECATED)) {
            m_cc = static_cast<int>( data[CFG_KEY_CC_DEPRECATED].as_integer());

            LOG_WARN << " --> Line " << data.location().line() << "  :: Parameter '" << CFG_KEY_CC_DEPRECATED
                     << "' is deprecated, please rename it to '" << CFG_KEY_CC << "'" << LOG_END

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CC_DEPRECATED << "' = '"
                      << m_cc << "'" << LOG_END
        } else {
            LOG_ERROR << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CC << "' is missing"
                      << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << " --> Line " << data.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}

} // Namespace xmidictrl