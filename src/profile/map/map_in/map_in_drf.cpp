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

#include "map_in_drf.h"

// XMidiCtrl
#include "conversions.h"
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_drf::map_in_drf(xplane &in_xp)
    : map_in(in_xp)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_drf::type()
{
    return map_type::dataref;
}


/**
 * Read settings from config
 */
void map_in_drf::read_config(text_logger &in_log, toml::value &in_data)
{
    in_log.debug(" --> Line %i :: Read settings for type 'drf'", in_data.location().line());
    map_in::read_config(in_log, in_data);

    // read mode
    m_mode = conversions::dataref_mode_from_code(toml_utils::read_string(in_log, in_data, CFG_KEY_MODE, false));

    // read dataref
    m_dataref = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF);

    // check if a values array has been defined
    m_values = toml_utils::read_str_vector_array(in_log, in_data, CFG_KEY_VALUES, false);

    if (m_values.empty()) {
        // read value on
        std::string value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_ON);

        if (!value.empty())
            m_values.push_back(value);

        // read value off
        if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_OFF, false))
            value = toml_utils::read_string(in_log, in_data, CFG_KEY_VALUE_OFF, false);

        if (!value.empty())
            m_values.push_back(value);
    }
}


/**
 * Check the mapping
 */
bool map_in_drf::check(text_logger &in_log)
{
    if (!map::check(in_log))
        return false;

    if (m_dataref.empty()) {
        in_log.error(source_line());
        in_log.error(" --> Parameter '%s' is empty", CFG_KEY_DATAREF);
        return false;
    }

    if (!xp().datarefs().check(in_log, m_dataref))
        return false;

    if (m_values.empty()) {
        in_log.error(source_line());
        in_log.error(" --> No values defined", CFG_KEY_DATAREF);
        return false;
    }

    if (m_mode == dataref_mode::momentary && m_values.size() != 2)
        return false;

    return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_drf::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    // if toggle mode then only process key pressed event
    if (m_mode == dataref_mode::toggle && in_msg.data_2() != MIDI_VELOCITY_MAX)
        return true;

    in_msg.log().debug(" --> Change dataref '%s'", m_dataref.c_str());

    if (m_values.size() == 2) {
        if (m_mode == dataref_mode::momentary) {
            if (in_msg.data_2() == MIDI_VELOCITY_MAX)
                xp().datarefs().write(in_msg.log(), m_dataref, m_values[0]);
            else
                xp().datarefs().write(in_msg.log(), m_dataref, m_values[1]);
        } else {
            xp().datarefs().toggle(in_msg.log(), m_dataref, m_values[0], m_values[1]);
        }
    } else {
        // get current value
        std::string value;
        xp().datarefs().read(in_msg.log(), m_dataref, value);

        // search for the value in the values list
        auto it = std::find(m_values.begin(), m_values.end(), value);

        if (it != m_values.end()) {
            int idx = static_cast<int>(std::distance(m_values.begin(), it));

            if (idx < m_values.size() - 1)
                idx++;
            else
                idx = 0;

            value = m_values[idx];
        } else {
            // values not found, let's take the first one of the list
            value = m_values[0];
        }

        xp().datarefs().write(in_msg.log(), m_dataref, value);
    }

    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_drf::build_mapping_text()
{
    std::string map_str = " :: Dataref ::\n";
    map_str.append("Dataref = '" + m_dataref + "'\n");

    if (m_mode == dataref_mode::toggle)
        map_str.append("Mode = 'toggle'\n");
    else
        map_str.append("Mode = 'momentary'\n");

    map_str.append("Values = [");

    for (auto &value : m_values)
        map_str.append(" '" + value + "'");

    map_str.append("]\n");

    return map_str;
}

} // Namespace xmidictrl