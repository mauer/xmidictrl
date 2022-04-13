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
}


/**
 * Read settings from config
 */
void map_in_sld::read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    in_log.debug(" --> Line %i :: Read settings for type 'sld'", in_data.location().line());
    map_in::read_config(in_log, in_data, in_config);

    // check if dataref was defined
    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF, false)) {
        in_log.debug(" --> Use 'dataref' mode for slider mapping");

        // read dataref
        m_dataref = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF);

        // read value min
        m_value_min = toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MIN, false, 0.0f);

        // read value max
        m_value_max = toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MAX, false, 1.0f);
    } else {
        in_log.debug(" --> Use 'command' mode for slider mapping");

        // read command up
        m_command_up = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_UP);

        // read command middle
        m_command_middle = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_MIDDLE, false);

        // read command down
        m_command_down = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_DOWN);
    }
}


/**
 * Check the mapping
 */
bool map_in_sld::check(text_logger &in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (!m_dataref.empty()) {
        // dataref mode
        if (!xp().datarefs().check(m_dataref)) {
            in_log.error(source_line());
            in_log.error(" --> Dataref '%s' not found", m_dataref.data());
            result = false;
        }

        if (m_value_min == m_value_max) {
            in_log.error(source_line());
            in_log.error(" --> Parameter '%s' is equal to parameter '%s'", CFG_KEY_VALUE_MIN, CFG_KEY_VALUE_MAX);
            result = false;
        }
    } else {
        // command mode
        if (m_command_up.empty() && m_command_down.empty()) {
            in_log.error(source_line());
            in_log.error(" --> Parameters '%s' and '%s' are not defined", CFG_KEY_COMMAND_UP, CFG_KEY_COMMAND_DOWN);
            result = false;
        }
    }

    return result;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_sld::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    if (!m_dataref.empty()) {
        // dataref mode
        float value = 0.0f;

        if (in_msg.data_2() == MIDI_VELOCITY_MIN)
            value = m_value_min;
        else if (in_msg.data_2() == MIDI_VELOCITY_MAX)
            value = m_value_max;
        else
            value = ((m_value_max - m_value_min) * (static_cast<float>(in_msg.data_2()) / 127.0f)) + m_value_min;

        in_msg.log().debug(" --> Set dataref '%s' to value '%f'", m_dataref.c_str(), value);

        if (xp().datarefs().write(in_msg.log(), m_dataref, value)) {
            try {
                display_label(in_msg.log(), std::to_string(value));
            } catch (std::bad_alloc &ex) {
                in_msg.log().error("Error converting float '%f' value to string", value);
                in_msg.log().error(ex.what());
            }
        }
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




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_sld::build_mapping_text()
{
    std::string map_str = " ====== Slider ======\n";

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

} // Namespace xmidictrl