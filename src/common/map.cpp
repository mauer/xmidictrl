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
map::map(xplane::ptr xplane, toml::value &settings)
    : m_xp(std::move(xplane))
{
    read_cc(settings);
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
mapping_type map::type()
{
    return mapping_type::none;
};


/**
 * Return the control change number
 */
int map::cc() const
{
    return m_cc;
}


/**
 * Check the mapping
 */
bool map::check()
{
    if (m_cc >= 0)
        return true;
    else
        return false;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter cc
 */
void map::read_cc(toml::value &settings)
{
    m_cc = -1;

    try {
        // read control change
        if (settings.contains(CFG_KEY_CC)) {
            m_cc = static_cast<int>( settings[CFG_KEY_CC].as_integer());

            LOG_DEBUG << "Line " << settings.location().line() << " :: Parameter '" << CFG_KEY_CC << "' = '"
                      << m_cc << "'" << LOG_END
        } else if (settings.contains(CFG_KEY_CC_DEPRECATED)) {
            m_cc = static_cast<int>( settings[CFG_KEY_CC_DEPRECATED].as_integer());

            LOG_WARN << "Device :: Mapping :: Line " << settings.location().line() << " Parameter 'CC' is deprecated, "
                     << "please rename it to 'cc'" << LOG_END
            LOG_DEBUG << "Device :: Mapping :: CC = '" << m_cc << "'" << LOG_END
        } else {
            LOG_ERROR << "Device :: Mapping :: Line " << settings.location().line() << " Parameter '" << CFG_KEY_CC
                      << "' is missing" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Device :: Mapping :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}

} // Namespace xmidictrl