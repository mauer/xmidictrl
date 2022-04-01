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
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map::map(xplane *in_xp)
    : m_xp(in_xp)
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
 * Return the mapping source line
 */
std::string_view map::source_line()
{
    return m_source_line;
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

    return conversions::create_map_key(m_channel, type_code, m_data);
}


/**
 * Read the config
 */
void map::read_config(text_logger *in_log, toml::value &in_data)
{
    // set source line
    m_source_line = in_data.location().line_str();

    // required config
    read_channel(in_log, in_data);
    read_data(in_log, in_data);
}


/**
 * Check the mapping
 */
bool map::check(text_logger *in_log)
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
void map::read_channel(text_logger *in_log, toml::value &in_data)
{
    m_channel = 11; // default channel 11

    try {
        // read channel
        if (in_data.contains(CFG_KEY_CH)) {
            m_channel = static_cast<unsigned char>( in_data[CFG_KEY_CH].as_integer());

            in_log->debug(" --> Line %i :: Parameter '%s' = '%i'", in_data.location().line(), CFG_KEY_CH, m_channel);
        } else {
            in_log->info(" --> Line %i :: Parameter '%s' is missing, will use default channel '11'",
                         in_data.location().line(),
                         CFG_KEY_CH);
            in_log->debug(" --> Line %i :: Parameter '%s' = '%i' (Default Value)",
                          in_data.location().line(),
                          CFG_KEY_CH,
                          m_channel);
        }
    } catch (toml::type_error &error) {
        in_log->error(" --> Line %i :: Error reading mapping", in_data.location().line());
        in_log->error(error.what());
    }
}


/**
 * Read parameter data
 */
void map::read_data(text_logger *in_log, toml::value &in_data)
{
    m_data = MIDI_NONE;
    m_data_type = map_data_type::none;

    try {
        // read control change
        if (in_data.contains(CFG_KEY_CC)) {
            m_data = static_cast<unsigned char>( in_data[CFG_KEY_CC].as_integer());
            m_data_type = map_data_type::control_change;

            in_log->debug(" --> Line %i :: Parameter '%s' = '%i'", in_data.location().line(), CFG_KEY_CC, m_data);
        } else if (in_data.contains(CFG_KEY_CC_DEPRECATED)) {
            m_data = static_cast<unsigned char>( in_data[CFG_KEY_CC_DEPRECATED].as_integer());
            m_data_type = map_data_type::control_change;

            in_log->warn(" --> Line %i :: Parameter '%s' is deprecated, please rename it to '%s'",
                         in_data.location().line(),
                         CFG_KEY_CC_DEPRECATED,
                         CFG_KEY_CC);

            in_log->debug(" --> Line %i :: Parameter '%s' = '%i'",
                          in_data.location().line(),
                          CFG_KEY_CC_DEPRECATED,
                          m_data);
        } else if (in_data.contains(CFG_KEY_NOTE)) {
            m_data = static_cast<unsigned char>( in_data[CFG_KEY_NOTE].as_integer());
            m_data_type = map_data_type::note;

            in_log->debug(" --> Line %i :: Parameter '%s' = '%i'", in_data.location().line(), CFG_KEY_NOTE, m_data);
        } else if (in_data.contains(CFG_KEY_PITCH_BEND)) {
            m_data = 0;   // fixed value for pitch bend messages
            m_data_type = map_data_type::pitch_bend;

            in_log->debug(" --> Line %i :: Parameter '%s' = '%i' (fixed value for pitch bend)",
                          in_data.location().line(),
                          CFG_KEY_PITCH_BEND,
                          m_data);
        } else if (in_data.contains(CFG_KEY_PROGRAM_CHANGE)) {
            m_data = static_cast<unsigned char>( in_data[CFG_KEY_PROGRAM_CHANGE].as_integer());
            m_data_type = map_data_type::program_change;

            in_log->debug(" --> Line %i :: Parameter '%s' = '%i'",
                          in_data.location().line(),
                          CFG_KEY_PROGRAM_CHANGE,
                          m_data);
        } else {
            in_log->error(" --> Line %i :: Parameter for MIDI type is missing", in_data.location().line());
        }

    } catch (toml::type_error &error) {
        in_log->error(" --> Line %i :: Error reading mapping", in_data.location().line());
        in_log->error(error.what());
    }
}

} // Namespace xmidictrl