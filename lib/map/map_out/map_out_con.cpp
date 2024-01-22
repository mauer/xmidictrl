//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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
void map_out_con::set_data_2(unsigned char in_data_2)
{
    if (in_data_2 <= MIDI_DATA_2_MAX)
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
    if (toml_utils::contains(in_log, in_data, c_cfg_velocity)) {
        // read velocity
        in_log.warn("File: " + in_data.location().file_name());
        in_log.warn_line(in_data.location().line(), in_data.location().line_str());
        in_log.warn_line(in_data.location().line(), " --> Parameter '" + std::string(c_cfg_velocity) + "' "
                                                    + "is depreciated and was replaced by parameter '"
                                                    + std::string(c_cfg_data_2) + "'");
        set_data_2(toml_utils::read_midi_value(in_log, in_data, c_cfg_velocity, MIDI_DATA_2_MIN));
    } else {
        // read data 2
        set_data_2(toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2, MIDI_DATA_2_MIN));
    }
}


/**
 * Check the mapping
 */
bool map_out_con::check(text_logger& in_log, const device_settings& in_dev_settings)
{
    bool result = true;

    if (!map::check(in_log, in_dev_settings))
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
std::unique_ptr<map_result> map_out_con::execute(map_param* in_param)
{
	auto result = std::make_unique<map_result>();
	result->data_changed = false;

	if (!check_sublayer(in_param->sl_value()))
        return result;

    switch (data_1_type()) {
        case map_data_1_type::control_change:
			result->type = midi_msg_type::control_change;
            break;

        case map_data_1_type::note:
            if (m_data_2 != MIDI_DATA_2_MIN)
				result->type = midi_msg_type::note_on;
            else
				result->type = midi_msg_type::note_off;
            break;

        case map_data_1_type::pitch_bend:
			result->type = midi_msg_type::pitch_bend;
            break;

        case map_data_1_type::program_change:
			result->type = midi_msg_type::program_change;
            break;

        case map_data_1_type::none:
			result->type = midi_msg_type::none;
            break;
    }

	result->channel = static_cast<char>(channel());
	result->data_1 = static_cast<char>(data_1());
	result->data_2 = static_cast<char>(m_data_2);

    return result;
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
