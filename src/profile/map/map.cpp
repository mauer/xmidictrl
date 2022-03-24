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
map::map(xplane *xp)
    : m_xp(xp)
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
unsigned char map::channel() const
{
    return m_channel;
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
        case map_data_type::none:
            type_code = "";
            break;

        case map_data_type::control_change:
            type_code = KEY_CONTROL_CHANGE;
            break;

        case map_data_type::note:
            type_code = KEY_NOTE;
            break;

        case map_data_type::pitch_bend:
            type_code = KEY_PITCH_BEND;
            break;

        case map_data_type::program_change:
            type_code = KEY_PROGRAM_CHANGE;
            break;
    }

    return utils::create_map_key(m_channel, type_code, m_data);
}


/**
 * Read the config
 */
void map::read_config(message_handler *messages, toml::value &data)
{
    // required config
    read_channel(messages, data);
    read_data(messages, data);
}


/**
 * Check the mapping
 */
bool map::check()
{
    if (m_channel != MIDI_NONE && m_data != MIDI_NONE && m_data_type != map_data_type::none)
        return true;
    else
        return false;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter channel
 */
void map::read_channel(message_handler *messages, toml::value &data)
{
    m_channel = 11; // default channel 11

    try {
        // read channel
        if (data.contains(CFG_KEY_CH)) {
            m_channel = static_cast<unsigned char>( data[CFG_KEY_CH].as_integer());

            messages->debug(" --> Line %i :: Parameter '%s' = '%i'", data.location().line(), CFG_KEY_CH, m_channel);
        } else {
            messages->info(" --> Line %i :: Parameter '%s' is missing, will use default channel '11'", data.location().line(), CFG_KEY_CH);
            messages->debug(" --> Line %i :: Parameter '%s' = '%i' (Default Value)", data.location().line(), CFG_KEY_CH, m_channel);
        }
    } catch (toml::type_error &error) {
        messages->error(" --> Line %i :: Error reading mapping", data.location().line());
        messages->error(error.what());
    }
}


/**
 * Read parameter data
 */
void map::read_data(message_handler *messages, toml::value &data)
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
                      << m_data << "'" << LOG_END
        } else if (data.contains(CFG_KEY_PITCH_BEND)) {
            m_data = 0;   // fixed value for pitch bend messages
            m_data_type = map_data_type::pitch_bend;

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_PITCH_BEND << "' = '"
                      << m_data << "' (fixed value for pitch bend)" << LOG_END
        } else if (data.contains(CFG_KEY_PROGRAM_CHANGE)) {
            m_data = static_cast<unsigned char>( data[CFG_KEY_PROGRAM_CHANGE].as_integer());
            m_data_type = map_data_type::program_change;

            LOG_DEBUG << " --> Line " << data.location().line() << " :: Parameter '" << CFG_KEY_PROGRAM_CHANGE
                      << "' = '" << m_data << "'" << LOG_END
        } else {
            LOG_ERROR << " --> Line " << data.location().line() << " :: Parameter for MIDI type is missing" << LOG_END
        }

    } catch (toml::type_error &error) {
        LOG_ERROR << " --> Line " << data.location().line() << " :: Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }
}

} // Namespace xmidictrl