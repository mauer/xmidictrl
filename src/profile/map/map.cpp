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
map::map(std::shared_ptr<xplane> xplane)
    : m_xp(std::move(xplane))
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
unsigned char map::ch() const
{
    return m_ch;
}


/**
 * Return the data type
 */
map_data_type map::data_type() const
{
    return m_data_type;
}


/**
 * Return the data number
 */
unsigned char map::data() const
{
    return m_data;
}


/**
 * Return a string containing channel, type and data
 */
std::string map::get_key()
{
    std::string type_code;

    switch (m_data_type) {
        case map_data_type::control_change:
            type_code = KEY_CONTROL_CHANGE;
            break;

        case map_data_type::note:
            type_code = KEY_NOTE;
            break;

        case map_data_type::program_change:
            type_code = KEY_PROGRAM_CHANGE;
            break;

        default:
            type_code = "";
            break;
    }

    return utils::create_map_key(ch(), type_code, data());
}


/**
 * Read the config
 */
void map::read_config(toml::value &data)
{
    // required config
    read_ch(data);
    read_data(data);
}


/**
 * Check the mapping
 */
bool map::check()
{
    if (m_ch != MIDI_NONE && m_data != MIDI_NONE && m_data_type != map_data_type::none)
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
            m_ch = static_cast<unsigned char>( data[CFG_KEY_CH].as_integer());

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
 * Read parameter data
 */
void map::read_data(toml::value &data)
{
    m_data = MIDI_NONE;
    m_data_type = map_data_type::none;

    try {
        // read control change
        if (data.contains(CFG_KEY_CC)) {
            m_data = static_cast<unsigned char>( data[CFG_KEY_CC].as_integer());
            m_data_type = map_data_type::control_change;

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CC << "' = '" << m_data
                      << "'" << LOG_END
        } else if (data.contains(CFG_KEY_CC_DEPRECATED)) {
            m_data = static_cast<unsigned char>( data[CFG_KEY_CC_DEPRECATED].as_integer());
            m_data_type = map_data_type::control_change;

            LOG_WARN << " --> Line " << data.location().line() << "  :: Parameter '" << CFG_KEY_CC_DEPRECATED
                     << "' is deprecated, please rename it to '" << CFG_KEY_CC << "'" << LOG_END

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CC_DEPRECATED << "' = '"
                      << m_data << "'" << LOG_END
        } else if (data.contains(CFG_KEY_NOTE)) {
            m_data = static_cast<unsigned char>( data[CFG_KEY_NOTE].as_integer());
            m_data_type = map_data_type::note;

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_NOTE << "' = '"
                      << m_data
                      << "'" << LOG_END
        } else {
            LOG_ERROR << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_CC << "' or "
                      << "Paramater '" << CFG_KEY_NOTE << "' is missing"
                      << LOG_END
        }

    } catch (toml::type_error &error) {
        LOG_ERROR << " --> Line " << data.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}

} // Namespace xmidictrl