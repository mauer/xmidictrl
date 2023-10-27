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

#include "map_out_drf.h"

// Standard
#include <utility>

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_out_drf::map_out_drf(environment& in_env)
    : map_out(in_env)
{}


/**
 * Destructor
 */
map_out_drf::~map_out_drf()
{
    m_datarefs.clear();

    m_xp_values.clear();

    m_values_on.clear();
    m_values_off.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_out_drf::type()
{
    return map_type::dataref;
}


/**
 * Set single value dataref
 */
void map_out_drf::set_dataref(std::string_view in_dataref)
{
    m_datarefs.clear();
    m_datarefs.emplace_back(in_dataref.data());
}


/**
 * Set multiple datarefs
 */
void map_out_drf::set_dataref(std::vector<std::string> in_dataref)
{
    m_datarefs = std::move(in_dataref);
}


/**
 * Set velocity on
 */
void map_out_drf::set_velocity_on(int in_velocity_on)
{
    if (in_velocity_on >= MIDI_VELOCITY_MIN && in_velocity_on <= MIDI_VELOCITY_MAX)
        m_velocity_on = in_velocity_on;
    else
        m_velocity_on = MIDI_VELOCITY_MAX;
}


/**
 * Set velocity off
 */
void map_out_drf::set_velocity_off(int in_velocity_off)
{
    if (in_velocity_off >= MIDI_VELOCITY_MIN && in_velocity_off <= MIDI_VELOCITY_MAX)
        m_velocity_off = in_velocity_off;
    else
        m_velocity_off = MIDI_VELOCITY_MIN;
}


/**
 * Read settings from config
 */
void map_out_drf::read_config(text_logger& in_log, toml::value& in_data)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'drf'");

    read_common_config(in_log, in_data);

    // read dataref
    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF)) {
        // check if single value or array
        if (in_data[CFG_KEY_DATAREF].is_array())
            set_dataref(toml_utils::read_str_vector_array(in_log, in_data, CFG_KEY_DATAREF));
        else
            set_dataref(toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF));
    }

    // read values on
    if (toml_utils::is_array(in_log, in_data, CFG_KEY_VALUE_ON)) {
        m_values_on = toml_utils::read_str_set_array(in_log, in_data, CFG_KEY_VALUE_ON, false);
    } else {
        m_values_on.clear();
        std::string value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_ON, false);

        if (!value.empty())
            m_values_on.insert(value);
    }

    // read values off
    if (toml_utils::is_array(in_log, in_data, CFG_KEY_VALUE_OFF)) {
        m_values_off = toml_utils::read_str_set_array(in_log, in_data, CFG_KEY_VALUE_OFF, false);
    } else {
        m_values_off.clear();
        std::string value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_OFF, false);

        if (!value.empty())
            m_values_off.insert(value);
    }

    // read velocity on
    set_velocity_on(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_ON, false));

    // read velocity off
    set_velocity_off(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_OFF, false));

    // read send on
    if (toml_utils::contains(in_log, in_data, CFG_KEY_SEND_ON, false)) {
        if (toml_utils::read_string(in_log, in_data, CFG_KEY_SEND_ON) == "all")
            m_send_on = send_mode::all;
    }

    // read send off
    if (toml_utils::contains(in_log, in_data, CFG_KEY_SEND_OFF, false)) {
        if (toml_utils::read_string(in_log, in_data, CFG_KEY_SEND_OFF) == "one")
            m_send_off = send_mode::one;
    }
}


/**
 * Check the mapping
 */
bool map_out_drf::check(text_logger& in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (m_datarefs.empty()) {
        in_log.error(source_line());
        in_log.error(" --> Parameter '" + std::string(CFG_KEY_DATAREF) + "' is not defined");
        result = false;
    }

    if (m_values_on.empty() && m_values_off.empty()) {
        in_log.error(source_line());
        in_log.error(" --> Parameters '" + std::string(CFG_KEY_VALUE_ON) + "' and '" + std::string(CFG_KEY_VALUE_OFF)
                     + "' are not defined");
        result = false;
    }

    for (const auto& dataref: m_datarefs) {
        if (!env().drf().check(dataref)) {
            in_log.error(source_line());
            in_log.error(" --> Dataref '" + std::string(dataref) + "' not found");
            result = false;
        }
    }

    if (m_velocity_on < MIDI_VELOCITY_MIN || m_velocity_on > MIDI_VELOCITY_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(CFG_KEY_VELOCITY_ON) + "', "
                     + "velocity has to be between " + std::to_string(MIDI_VELOCITY_MIN) + " and "
                     + std::to_string(MIDI_VELOCITY_MAX));
        result = false;
    }

    if (m_velocity_off < MIDI_VELOCITY_MIN || m_velocity_off > MIDI_VELOCITY_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(CFG_KEY_VELOCITY_OFF) + "', "
                     + "velocity has to be between " + std::to_string(MIDI_VELOCITY_MIN) + " and "
                     + std::to_string(MIDI_VELOCITY_MAX));
        result = false;
    }

    return result;
}


/**
 * Create a MIDI outbound task if required
 */
std::shared_ptr<outbound_task> map_out_drf::execute(text_logger& in_log,
                                                    outbound_send_mode in_send_mode,
                                                    std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return {};

    bool changed = false;

    bool send_msg = false;
    int send_on_cnt = 0;
    int send_off_cnt = 0;

    // if one value has been changed, all other values have to be checked as well
    for (auto& dataref: m_datarefs) {
        // get the current value from X-Plane
        std::string value_current;

        if (!env().drf().read(in_log, dataref, value_current))
            continue;

        // get current value
        std::string value_previous;
        if (m_xp_values.contains(dataref)) {
            value_previous = m_xp_values[dataref];
            m_xp_values[dataref] = value_current;
        } else {
            m_xp_values.emplace(dataref, value_current);
        }

        if (value_current != value_previous)
            changed = true;

        if (in_send_mode == outbound_send_mode::on_change) {
            if (changed)
                send_msg = true;
        } else if (in_send_mode == outbound_send_mode::permanent) {
            send_msg = true;
        }
    }

    if (!send_msg)
        return {};

    // alright, some have been changed, let's check what we have to send out
    for (auto& dataref: m_datarefs) {
        std::string value_current = m_xp_values[dataref];

        // value_on has been defined
        if (!m_values_on.empty()) {
            if (m_values_on.find(value_current) != m_values_on.end()) {
                send_on_cnt++;
                continue;
            } else if (m_values_off.find(value_current) != m_values_off.end() || m_values_off.empty()) {
                send_off_cnt++;
            }
        } else {
            if (m_values_off.find(value_current) != m_values_off.end()) {
                send_off_cnt++;
            } else if (m_values_on.find(value_current) != m_values_on.end() || m_values_on.empty()) {
                send_on_cnt++;
                continue;
            }
        }
    }

    if ((m_send_on == send_mode::all && send_on_cnt == m_datarefs.size())
        || (m_send_on == send_mode::one && send_on_cnt > 0)
        || (m_send_off == send_mode::all && send_off_cnt == m_datarefs.size())
        || (m_send_off == send_mode::one && send_off_cnt > 0)) {
        std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

        task->data_changed = changed;

        switch (data_type()) {
            case map_data_type::control_change:
                task->type = midi_msg_type::control_change;
                break;

            case map_data_type::note:
                if ((m_send_on == send_mode::all && send_on_cnt == m_datarefs.size())
                    || (m_send_on == send_mode::one && send_on_cnt > 0))
                    task->type = midi_msg_type::note_on;
                else
                    task->type = midi_msg_type::note_off;
                break;

            case map_data_type::pitch_bend:
                task->type = midi_msg_type::pitch_bend;
                break;

            case map_data_type::program_change:
                task->type = midi_msg_type::program_change;
                break;

            case map_data_type::none:
                task->type = midi_msg_type::none;
                break;
        }

        task->channel = channel();
        task->data = data();

        if ((m_send_on == send_mode::all && send_on_cnt == m_datarefs.size())
            || (m_send_on == send_mode::one && send_on_cnt > 0))
            task->velocity = static_cast<unsigned char>(m_velocity_on);
        else
            task->velocity = static_cast<unsigned char>(m_velocity_off);


        // add mapping to task
        task->mapping = shared_from_this();

        return task;
    }

    return {};
}


/**
 * Reset the lights on the MIDI device
 */
std::shared_ptr<outbound_task> map_out_drf::reset()
{
    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = true;

    switch (data_type()) {
        case map_data_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_type::note:
            task->type = midi_msg_type::note_off;
            break;

        case map_data_type::pitch_bend:
            task->type = midi_msg_type::pitch_bend;
            break;

        case map_data_type::program_change:
            task->type = midi_msg_type::program_change;
            break;

        case map_data_type::none:
            task->type = midi_msg_type::none;
            break;
    }

    task->channel = channel();
    task->data = data();

    task->velocity = MIDI_VELOCITY_MIN;

    return task;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_out_drf::build_mapping_text()
{
    std::string map_str = " ====== Dataref ======\n";

    // Dataref
    if (m_datarefs.size() == 1) {
        map_str.append("Dataref = '" + m_datarefs[0] + "'\n");
    } else {
        map_str.append("Datarefs = [");

        std::string data_str;
        for (auto& str: m_datarefs) {
            if (!data_str.empty())
                data_str.append(", ");

            data_str.append("'" + str + "'");
        }

        map_str.append(data_str + "]\n");
    }

    // Values on
    if (m_values_on.size() == 1) {
        map_str.append("Value on = '" + *m_values_on.begin() + "'\n");
    } else if (m_values_on.size() > 1) {
        map_str.append("Values on = [");

        std::string values_str;
        for (auto& str: m_values_on) {
            if (!values_str.empty())
                values_str.append(", ");

            values_str.append("'" + str + "'");
        }

        map_str.append(values_str + "]\n");
    }

    // Values off
    if (m_values_off.size() == 1) {
        map_str.append("Value off = '" + *m_values_off.begin() + "'\n");
    } else if (m_values_off.size() > 1) {
        map_str.append("Values off = [");

        std::string values_str;
        for (auto& str: m_values_off) {
            if (!values_str.empty())
                values_str.append(", ");

            values_str.append("'" + str + "'");
        }

        map_str.append(values_str + "]\n");
    }

    // Velocity on
    if (m_velocity_on != MIDI_VELOCITY_MAX)
        map_str.append("Velocity on = '" + std::to_string(m_velocity_on) + "'\n");

    // Velocity off
    if (m_velocity_off != MIDI_VELOCITY_MIN)
        map_str.append("Velocity off = '" + std::to_string(m_velocity_off) + "'\n");

    // Send on
    if (m_send_on == send_mode::all)
        map_str.append("Send on = 'all'\n");
    else
        map_str.append("Send on = 'one'\n");

    // Send off
    if (m_send_off == send_mode::all)
        map_str.append("Send off = 'all'");
    else
        map_str.append("Send off = 'one'");

    return map_str;
}

} // Namespace xmidictrl
