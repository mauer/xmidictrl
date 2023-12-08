//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#include "map_in_cmd.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_cmd::map_in_cmd(environment& in_env)
    : map_in_label(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_cmd::type()
{
    return map_in_type::command;
}


/**
 * Return data 2 on
 */
unsigned char map_in_cmd::data_2_on() const
{
    return m_data_2_on;
}


/**
 * Return data 2 off
 */
unsigned char map_in_cmd::data_2_off() const
{
    return m_data_2_off;
}


/**
 * Read settings from config
 */
void map_in_cmd::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'cmd'");
    map_in_label::read_config(in_log, in_data, in_config);

    // read the command
    m_command = toml_utils::read_string(in_log, in_data, c_cfg_command);

    // read data_2 parameters
    m_data_2_on = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_on, MIDI_DATA_2_MAX);
    m_data_2_off = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_off, MIDI_DATA_2_MIN);
}


/**
 * Check the mapping
 */
bool map_in_cmd::check(text_logger& in_log)
{
    if (!map::check(in_log))
        return false;

    if (m_command.empty()) {
        in_log.error(source_line());
        in_log.error(fmt::format(" --> Parameter '{}' is empty", c_cfg_command));
        return false;
    }

    if (m_data_2_on > MIDI_DATA_2_MAX) {
        in_log.error(source_line());
        in_log.error(fmt::format(" --> Invalid value for parameter '{}', it has to be between {} and {}", c_cfg_data_2_on, MIDI_DATA_2_MIN, MIDI_DATA_2_MAX));

        return false;
    }

    if (m_data_2_off > MIDI_DATA_2_MAX) {
        in_log.error(source_line());
        in_log.error(fmt::format(" --> Invalid value for parameter '{}', it has to be between {} and {}", c_cfg_data_2_off, MIDI_DATA_2_MIN, MIDI_DATA_2_MAX));

        return false;
    }

    return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_cmd::execute(midi_message& in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    in_msg.log().debug(" --> Execute command '" + m_command + "'");

    if (in_msg.data_2() == m_data_2_on) {
        env().cmd().begin(in_msg.log(), m_command);
    } else if (in_msg.data_2() == m_data_2_off) {
        env().cmd().end(in_msg.log(), m_command);
    } else {
        in_msg.log().error("Invalid MIDI velocity '" + std::to_string(in_msg.data_2()) + "'");
        in_msg.log().error(
            " --> Supported values for the current mapping are '" + std::to_string(m_data_2_on) + "' and '"
            + std::to_string(m_data_2_off) + "'");
    }

    return true;
}


/**
 * Return mapped command
 */
std::string map_in_cmd::map_text_cmd_drf()
{
    return m_command;
}


/**
 * Return mapped parameter
 */
std::string map_in_cmd::map_text_parameter()
{
    std::string map_str {};

    // Data 2 on/off
    if (m_data_2_on != MIDI_DATA_2_MAX)
        map_str.append("Data 2 on = " + std::to_string(m_data_2_on));

    if (m_data_2_off != MIDI_DATA_2_MIN) {
        if (!map_str.empty())
            map_str.append("   |   ");

        map_str.append("Data 2 off = " + std::to_string(m_data_2_off));
    }

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_cmd::build_mapping_text(bool in_short)
{
    std::string map_str {};
    std::string sep_str {", "};

    if (!in_short) {
        sep_str = "\n";
        map_str = " ====== Command ======" + sep_str;

        if (!sl().empty())
            map_str.append("Sublayer = '" + std::string(sl()) + "'" + sep_str);
    }

    if (!in_short)
        map_str.append("Command: ");

    map_str.append(m_command);

    if (m_data_2_on != MIDI_DATA_2_MAX)
        map_str.append(sep_str + "Data 2: " + std::to_string(m_data_2_on));

    if (m_data_2_off != MIDI_DATA_2_MIN)
        map_str.append(sep_str + "Data 2: " + std::to_string(m_data_2_off));

    return map_str;
}

}// Namespace xmidictrl