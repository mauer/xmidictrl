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
 * Return the sublayer name
 */
std::string_view map::sl() const
{
    return m_sl;
}


/**
 * Read the config
 */
void map::read_config(toml::value &data)
{
    // required config
    read_ch(data);
    read_cc(data);

    // additional config
    read_sl(data);
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


/**
 * Read parameter sl
 */
void map::read_sl(toml::value &data)
{
    m_sl.clear();

    try {
        // read sublayer
        if (data.contains(CFG_KEY_SL)) {
            m_sl = data[CFG_KEY_SL].as_string();

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_SL << "' = '" << m_sl
                      << "'" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << " --> Line " << data.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}


/**
 * Check if the command is defined for the current sublayer
 */
bool map::check_sublayer(std::string_view sl_value)
{
    if (sl_value != m_sl && !m_sl.empty())
        return false;

    return true;
}

} // Namespace xmidictrl