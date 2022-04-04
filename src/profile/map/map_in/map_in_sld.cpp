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

#include "map_in_sld.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_sld::map_in_sld(xplane &in_xp)
    : map_in(in_xp)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_sld::type()
{
    return map_type::slider;
};


/**
 * Return the mapping as string
 */
std::string map_in_sld::as_string()
{
    std::string map_str = " :: Slider ::\n";

    if (!m_dataref.empty()) {
        map_str.append("Dataref = '" + m_dataref + "'\n");
        map_str.append("Value min = '" + std::to_string(m_value_min) + "\n");
        map_str.append("Value max = '" + std::to_string(m_value_max) + "'\n");
    } else {
        if (!m_command_middle.empty())
            map_str.append("Command down = '" + m_command_down + "'\nCommand middle = '" + m_command_middle +
                           "'\nCommand up = '" + m_command_up + "'\n");
        else
            map_str.append("Command down = '" + m_command_down + "'\nCommand up = '" + m_command_up + "'\n");
    }

    return map_str;
}


/**
 * Set the dataref
 */
void map_in_sld::set_dataref(std::string_view in_dataref)
{
    m_dataref = in_dataref;
}


/**
 * Return the dataref
 */
std::string_view map_in_sld::dataref() const
{
    return m_dataref;
}


/**
 * Set the min value
 */
void map_in_sld::set_value_min(float in_value)
{
    m_value_min = in_value;
}


/**
 * Return the min value
 */
float map_in_sld::value_min() const
{
    return m_value_min;
}


/**
 * Set the max value
 */
void map_in_sld::set_value_max(float in_value)
{
    m_value_max = in_value;
}


/**
 * Return the max value
 */
float map_in_sld::value_max() const
{
    return m_value_max;
}


/**
 * Set the up command
 */
void map_in_sld::set_command_up(std::string_view in_command)
{
    m_command_up = in_command;
}


/**
 * Return the up command
 */
std::string_view map_in_sld::command_up() const
{
    return m_command_up;
}


/**
 * Set the middle command
 */
void map_in_sld::set_command_middle(std::string_view in_command)
{
    m_command_middle = in_command;
}


/**
 * Return the middle command
 */
std::string_view map_in_sld::command_middle() const
{
    return m_command_middle;
}


/**
 * Set the down command
 */
void map_in_sld::set_command_down(std::string_view in_command)
{
    m_command_down = in_command;
}


/**
 * Return the down command
 */
std::string_view map_in_sld::command_down() const
{
    return m_command_down;
}


/**
 * Read settings from config
 */
void map_in_sld::read_config(text_logger &in_log, toml::value &in_data)
{
    in_log.debug(" --> Line %i :: Read settings for type 'sld'", in_data.location().line());
    map_in::read_config(in_log, in_data);

    // check if dataref was defined
    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF, false)) {
        in_log.debug(" --> Use 'dataref' mode for slider mapping");

        // read dataref
        set_dataref(toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF));

        // read value min
        set_value_min(toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MIN, false, 0.0f));

        // read value max
        set_value_max(toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MAX, false, 1.0f));
    } else {
        in_log.debug(" --> Use 'command' mode for slider mapping");

        // read command up
        set_command_up(toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_UP));

        // read command middle
        set_command_middle(toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_MIDDLE, false));

        // read command down
        set_command_down(toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_DOWN));
    }
}


/**
 * Check the mapping
 */
bool map_in_sld::check(text_logger &in_log)
{
    if (!map::check(in_log))
        return false;

    if (!dataref().empty()) {
        // dataref mode
        if (!xp().datarefs().check(in_log, dataref()))
            return false;

        if (value_min() == value_max())
            return false;
    } else {
        // command mode
        if (m_command_up.empty() && m_command_down.empty())
            return false;
    }

    return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_sld::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    if (!dataref().empty()) {
        // dataref mode
        float value = 0.0f;
        if (in_msg.data_2() == MIDI_VELOCITY_MIN)
            value = m_value_min;
        else if (in_msg.data_2() == MIDI_VELOCITY_MAX)
            value = m_value_max;
        else
            value = ((m_value_max - m_value_min) * (static_cast<float>(in_msg.data_2()) / 127.0f)) + m_value_min;

        in_msg.log().debug(" --> Set dataref '%s' to value '%f'", m_dataref.c_str(), value);
        xp().datarefs().write(in_msg.log(), m_dataref, value);
    } else {
        // command mode
        if (in_msg.data_2() <= 10) {
            in_msg.log().debug(" --> Execute command '%s'", m_command_down.c_str());

            if (m_command_down != m_command_prev)
                xp().cmd().execute(in_msg.log(), m_command_down);

            m_command_prev = m_command_down;

        } else if (in_msg.data_2() >= 117) {
            in_msg.log().debug(" --> Execute command '%s'", m_command_up.c_str());

            if (m_command_up != m_command_prev)
                xp().cmd().execute(in_msg.log(), m_command_up);

            m_command_prev = m_command_up;

        } else if (in_msg.data_2() >= 50 && in_msg.data_2() <= 70) {
            if (!m_command_middle.empty()) {
                in_msg.log().debug(" --> Execute command '%s'", m_command_middle.c_str());

                if (m_command_middle != m_command_prev)
                    xp().cmd().execute(in_msg.log(), m_command_middle);

                m_command_prev = m_command_middle;
            }
        } else {
            // clear previous command
            m_command_prev.clear();
        }
    }

    return true;
}

} // Namespace xmidictrl