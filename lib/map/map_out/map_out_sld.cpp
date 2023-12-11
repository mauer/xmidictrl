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

#include "map_out_sld.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_out_sld::map_out_sld(environment& in_env)
    : map_out(in_env)
{}


/**
 * Destructor
 */
map_out_sld::~map_out_sld()
{
    m_dataref.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_out_type map_out_sld::type()
{
    return map_out_type::slider;
}


/**
 * Set the dataref
 */
void map_out_sld::set_dataref(std::string_view in_dataref)
{
    m_dataref = in_dataref;
}


/**
 * Set min value
 */
void map_out_sld::set_value_min(float in_value_min)
{
    m_value_min = in_value_min;
}


/**
 * Set max value
 */
void map_out_sld::set_value_max(float in_value_max)
{
    m_value_max = in_value_max;
}


/**
 * Set min data 2
 */
void map_out_sld::set_data_2_min(int in_data_2_min)
{
    if (in_data_2_min >= MIDI_DATA_2_MIN && in_data_2_min <= MIDI_DATA_2_MAX)
        m_data_2_min = in_data_2_min;
    else
        m_data_2_min = MIDI_DATA_2_MIN;
}


/**
 * Set max data 2
 */
void map_out_sld::set_data_2_max(int in_data_2_max)
{
    if (in_data_2_max >= MIDI_DATA_2_MIN && in_data_2_max <= MIDI_DATA_2_MAX)
        m_data_2_max = in_data_2_max;
    else
        m_data_2_max = MIDI_DATA_2_MIN;
}


/**
 * Read settings from config
 */
void map_out_sld::read_config(text_logger& in_log, toml::value& in_data)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'sld'");

    read_common_config(in_log, in_data);

    // read dataref
    set_dataref(toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF));

    // read value min
    if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_MIN))
        set_value_min(toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MIN, false));

    // read value max
    if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_MAX))
        set_value_max(toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MAX, false));

    // read data 2 min
    if (toml_utils::contains(in_log, in_data, c_cfg_data_2_min))
        set_data_2_min(toml_utils::read_int(in_log, in_data, c_cfg_data_2_min));

    // read data 2 max
    if (toml_utils::contains(in_log, in_data, c_cfg_data_2_max))
        set_data_2_max(toml_utils::read_int(in_log, in_data, c_cfg_data_2_max));
}


/**
 * Check the mapping
 */
bool map_out_sld::check(text_logger& in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (m_dataref.empty()) {
        in_log.error(source_line());
        in_log.error(" --> Parameter '" + std::string(CFG_KEY_DATAREF) + "' is not defined");
        result = false;
    }

    if (!env().drf().check(m_dataref)) {
        in_log.error(source_line());
        in_log.error(" --> Dataref '" + std::string(m_dataref) + "' not found");
        result = false;
    }

    if (m_value_min == m_value_max) {
        in_log.error(source_line());
        in_log.error(" --> Parameter '" + std::string(CFG_KEY_VALUE_MIN) + "' is equal to parameter '"
                     + std::string(CFG_KEY_VALUE_MAX) + "'");
        result = false;
    }

    if (m_data_2_min < MIDI_DATA_2_MIN || m_data_2_min > MIDI_DATA_2_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(c_cfg_data_2_min) + "', "
                     + "it has to be between " + std::to_string(MIDI_DATA_2_MIN) + " and "
                     + std::to_string(MIDI_DATA_2_MAX));
        result = false;
    }

    if (m_data_2_max < MIDI_DATA_2_MIN || m_data_2_max > MIDI_DATA_2_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(c_cfg_data_2_max) + "', "
                     + "it has to be between " + std::to_string(MIDI_DATA_2_MIN) + " and "
                     + std::to_string(MIDI_DATA_2_MAX));
        result = false;
    }

    return result;
}


/**
 * Create a MIDI outbound task if required
 */
std::shared_ptr<outbound_task> map_out_sld::execute(text_logger& in_log,
                                                    outbound_send_mode in_send_mode,
                                                    std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return {};

    bool changed = false;
    bool send_msg = false;

    // get the current value from X-Plane, but save the current value beforehand
    float value_previous = m_xp_value;

    if (!env().drf().read(in_log, m_dataref, m_xp_value))
        return {};

    if ((m_xp_value != value_previous) || m_first_execution)
        changed = true;

    if (m_first_execution)
        m_first_execution = false;

    if (in_send_mode == outbound_send_mode::on_change) {
        if (changed)
            send_msg = true;
    } else if (in_send_mode == outbound_send_mode::permanent) {
        send_msg = true;
    }

    if (!send_msg)
        return {};

    // alright, dataref has been changed, let's check what we have to send out
    float percent_value = (m_xp_value - m_value_min) * 100 / (m_value_max - m_value_min);
    float data_2 = static_cast<float>((m_data_2_max - m_data_2_min)) * percent_value / 100
                   + static_cast<float>(m_data_2_min);

    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = changed;

    switch (data_1_type()) {
        case map_data_1_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_1_type::note:
            if (static_cast<unsigned int>(data_2) == m_data_2_max)
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
    task->data_2 = static_cast<unsigned char>(data_2);

    // add mapping to task
    task->mapping = this;

    return task;
}


/**
 * Reset the lights on the MIDI device
 */
std::shared_ptr<outbound_task> map_out_sld::reset()
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
std::string map_out_sld::map_text_drf()
{
    return m_dataref;
}


/**
 * Return mapped parameter
 */
std::string map_out_sld::map_text_parameter()
{
    std::string map_str = "Value min = " + std::to_string(m_value_min);
    map_str.append("   |   ");
    map_str.append("Value max = " + std::to_string(m_value_max));

    // Data 2 min/max
    if (m_data_2_min != MIDI_DATA_2_MAX)
        map_str.append("   |   Data 2 min = " + std::to_string(m_data_2_min));

    if (m_data_2_max != MIDI_DATA_2_MIN)
        map_str.append("   |   Data 2 max = " + std::to_string(m_data_2_max));

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_out_sld::build_mapping_text(bool in_short)
{
    std::string map_str {};
    std::string sep_str {", "};

    if (!in_short) {
        sep_str = "\n";
        map_str = " ====== Slider ======" + sep_str;
    }

    // Dataref
    map_str.append("Dataref = '" + m_dataref + "'" + sep_str);

    // Value min/max
    map_str.append("Value min = " + std::to_string(m_value_min) + sep_str);
    map_str.append("Value max = " + std::to_string(m_value_max));

    // Data 2 min/max
    if (m_data_2_min != MIDI_DATA_2_MIN)
        map_str.append(sep_str + "Data 2 min = " + std::to_string(m_data_2_min));

    if (m_data_2_max != MIDI_DATA_2_MAX)
        map_str.append(sep_str + "Data 2 max = " + std::to_string(m_data_2_max));

    return map_str;
}

} // Namespace xmidictrl
