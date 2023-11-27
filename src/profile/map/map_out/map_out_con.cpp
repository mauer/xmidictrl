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

#include "map_out_con.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_out_con::map_out_con(environment& in_env)
    : map_out(in_env)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_out_type map_out_con::type()
{
    return map_out_type::constant;
}


/**
 * Set the data 2 value
 */
void map_out_con::set_data_2(int in_data_2)
{
    if (in_data_2 >= MIDI_DATA_2_MIN && in_data_2 <= MIDI_DATA_2_MAX)
        m_data_2 = in_data_2;
    else
        m_data_2 = MIDI_DATA_2_MAX;
}


/**
 * Read settings from config
 */
void map_out_con::read_config(text_logger& in_log, toml::value& in_data)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'con'");

    read_common_config(in_log, in_data);

    // check for depreciated velocity parameter
    if (toml_utils::contains(in_log, in_data, c_cfg_velocity, false)) {
        // read velocity
        in_log.warn("File: " + in_data.location().file_name());
        in_log.warn_line(in_data.location().line(), in_data.location().line_str());
        in_log.warn_line(in_data.location().line(), " --> Parameter '" + std::string(c_cfg_velocity) + "' "
                                                    + "is depreciated and was replaced by parameter '"
                                                    + std::string(c_cfg_data_2) + "'");
        set_data_2(toml_utils::read_int(in_log, in_data, c_cfg_velocity, true));
    } else {
        // read data 2
        set_data_2(toml_utils::read_int(in_log, in_data, c_cfg_data_2, true));
    }
}


/**
 * Check the mapping
 */
bool map_out_con::check(text_logger& in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (m_data_2 < MIDI_DATA_2_MIN || m_data_2 > MIDI_DATA_2_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(c_cfg_data_2) + "', "
                     + "it has to be between " + std::to_string(MIDI_DATA_2_MIN) + " and "
                     + std::to_string(MIDI_DATA_2_MAX));

        result = false;
    }

    return result;
}


/**
 * Create a MIDI outbound task if required
 */
std::shared_ptr<outbound_task> map_out_con::execute(text_logger&, outbound_send_mode, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return {};

    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = false;

    switch (data_1_type()) {
        case map_data_1_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_1_type::note:
            if (m_data_2 != MIDI_DATA_2_MIN)
                task->type = midi_msg_type::note_on;
            else
                task->type = midi_msg_type::note_off;
            break;

        case map_data_1_type::pitch_bend:
            task->type = midi_msg_type::pitch_bend;
            break;

        case map_data_1_type::program_change:
            task->type = midi_msg_type::program_change;
            break;

        case map_data_1_type::none:
            task->type = midi_msg_type::none;
            break;
    }

    task->channel = channel();
    task->data_1 = data_1();
    task->data_2 = static_cast<char>(m_data_2);

    // add mapping to task
    task->mapping = shared_from_this();

    return task;
}


/**
 * Reset the lights on the MIDI device
 */
std::shared_ptr<outbound_task> map_out_con::reset()
{
    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = true;

    switch (data_1_type()) {
        case map_data_1_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_1_type::note:
            task->type = midi_msg_type::note_off;
            break;

        case map_data_1_type::pitch_bend:
            task->type = midi_msg_type::pitch_bend;
            break;

        case map_data_1_type::program_change:
            task->type = midi_msg_type::program_change;
            break;

        case map_data_1_type::none:
            task->type = midi_msg_type::none;
            break;
    }

    task->channel = channel();
    task->data_1 = data_1();
    task->data_2 = MIDI_DATA_2_MIN;

    return task;
}


/**
 * Return mapped dataref
 */
std::string map_out_con::map_text_drf()
{
    return {};
}


/**
 * Return mapped parameter
 */
std::string map_out_con::map_text_parameter()
{
    return "Data 2 = " + std::to_string(m_data_2);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_out_con::build_mapping_text(bool in_short)
{
    std::string map_str;

    if (!in_short)
        map_str = " ====== Constant ======\n";

    map_str.append("Data 2: " + std::to_string(m_data_2));

    return map_str;
}

} // Namespace xmidictrl
