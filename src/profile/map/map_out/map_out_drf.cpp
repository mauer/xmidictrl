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
map_out_drf::map_out_drf(xplane *xp)
    : map_out(xp)
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
};


/**
 * Set single value dataref
 */
void map_out_drf::set_dataref(std::string_view dataref)
{
    m_datarefs.clear();
    m_datarefs.emplace_back(dataref.data());
}


/**
 * Set multiple datarefs
 */
void map_out_drf::set_dataref(std::vector<std::string> dataref)
{
    m_datarefs = std::move(dataref);
}


/**
 * Set values on
 */
void map_out_drf::set_values_on(const std::set<std::string> &values)
{
    m_values_on = values;
}


/**
 * Return value on
 */
std::set<std::string> map_out_drf::values_on() const
{
    return m_values_on;
}


/**
 * Set values off
 */
void map_out_drf::set_values_off(const std::set<std::string> &values)
{
    m_values_off = values;
}


/**
 * Return value off
 */
std::set<std::string> map_out_drf::values_off() const
{
    return m_values_off;
}


/**
 * Set velocity on
 */
void map_out_drf::set_velocity_on(int velocity_on)
{
    if (velocity_on >= MIDI_VELOCITY_MIN && velocity_on <= MIDI_VELOCITY_MAX)
        m_velocity_on = velocity_on;
    else
        m_velocity_on = MIDI_VELOCITY_MAX;
}


/**
 * Return velocity on
 */
unsigned int map_out_drf::velocity_on() const
{
    return m_velocity_on;
}


/**
 * Set velocity off
 */
void map_out_drf::set_velocity_off(int velocity_off)
{
    if (velocity_off >= MIDI_VELOCITY_MIN && velocity_off <= MIDI_VELOCITY_MAX)
        m_velocity_off = velocity_off;
    else
        m_velocity_off = MIDI_VELOCITY_MIN;
}


/**
 * Return velocity off
 */
unsigned int map_out_drf::velocity_off() const
{
    return m_velocity_off;
}


/**
 * Read settings from config
 */
void map_out_drf::read_config(text_logger *in_log, toml::value &in_data)
{
    in_log->debug("Line %i :: Read settings for type 'drf'", in_data.location().line());
    map::read_config(in_log, in_data);

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
        set_values_on(toml_utils::read_str_set_array(in_log, in_data, CFG_KEY_VALUE_ON, false));
    } else {
        std::set<std::string> list;
        std::string value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_ON, false);

        if (!value.empty())
            list.insert(value);

        set_values_on(list);
    }

    // read values off
    if (toml_utils::is_array(in_log, in_data, CFG_KEY_VALUE_OFF)) {
        set_values_off(toml_utils::read_str_set_array(in_log, in_data, CFG_KEY_VALUE_OFF, false));
    } else {
        std::set<std::string> list;
        std::string value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_OFF, false);

        if (!value.empty())
            list.insert(value);

        set_values_off(list);
    }

    // read velocity on
    set_velocity_on(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_ON, false));

    // read velocity off
    set_velocity_off(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY_OFF, false));
}


/**
 * Check the mapping
 */
bool map_out_drf::check(text_logger *in_log)
{
    if (!map::check(in_log))
        return false;

    if (m_datarefs.empty())
        return false;

    if (m_values_on.empty() && m_values_off.empty())
        return false;

    for (const auto &dataref: m_datarefs) {
        if (!m_xp->datarefs().check(in_log, dataref))
            return false;
    }

    return true;
}


/**
 * Create a MIDI outbound task if required
 */
std::shared_ptr<outbound_task> map_out_drf::execute(text_logger *in_log, const mode_out mode)
{
    bool changed = false;

    bool send_msg = false;
    bool send_on = false;
    char send_off = 0;

    // if one value has been changed, all other values have to be checked as well
    for (auto &dataref: m_datarefs) {
        // get the current value from X-Plane
        std::string value_current;

        if (!m_xp->datarefs().read(in_log, dataref, value_current))
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

        if (mode == mode_out::on_change) {
            if (changed)
                send_msg = true;
        } else if (mode == mode_out::permanent) {
            send_msg = true;
        }
    }

    if (!send_msg)
        return {};

    // alright, some has been changed, let's check what we have to send out
    for (auto &dataref: m_datarefs) {
        std::string value_current = m_xp_values[dataref];

        // value_on has been defined
        if (!m_values_on.empty()) {
            if (m_values_on.find(value_current) != m_values_on.end()) {
                send_on = true;
                break;
            } else if (m_values_off.find(value_current) != m_values_off.end() || m_values_off.empty()) {
                send_off++;
            }
        } else {
            if (m_values_off.find(value_current) != m_values_off.end()) {
                send_off++;
            } else if (m_values_on.find(value_current) != m_values_on.end() || m_values_on.empty()) {
                send_on = true;
                break;
            }
        }
    }

    if (send_on || send_off == m_datarefs.size()) {
        std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

        task->data_changed = changed;

        switch (data_type()) {
            case map_data_type::control_change:
                task->type = midi_msg_type::control_change;
                break;

            case map_data_type::note:
                if (send_on)
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

        task->ch = channel();
        task->data = data();

        if (send_on)
            task->velocity = m_velocity_on;
        else
            task->velocity = m_velocity_off;

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

    task->ch = channel();
    task->data = data();

    task->velocity = MIDI_VELOCITY_MIN;

    return task;
}

} // Namespace xmidictrl