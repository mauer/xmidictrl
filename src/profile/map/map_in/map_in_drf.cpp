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
map_in_drf::map_in_drf(environment &in_env)
    : map_in(in_env)
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
void map_in_drf::read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    in_log.debug(" --> Line " + std::to_string(in_data.location().line()) + " :: Read settings for type 'drf'");
    map_in::read_config(in_log, in_data, in_config);

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
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (m_dataref.empty()) {
        in_log.error(source_line());
        in_log.error(" --> Parameter '" + std::string(CFG_KEY_DATAREF) + "' is empty");
        result = false;
    }

    if (!env().drf().check(m_dataref)) {
        in_log.error(source_line());
        in_log.error(" --> Dataref '" + m_dataref + "' not found");
        result = false;
    }

    if (m_values.empty()) {
        in_log.error(source_line());
        in_log.error(" --> No values defined");
        result = false;
    }

    if (m_mode == dataref_mode::momentary && m_values.size() != 2) {
        in_log.error(source_line());
        in_log.error(" --> When parameter '" + std::string(CFG_KEY_MODE) + "' is 'momentary', two values (on/off) are expected");
        result = false;
    }

    return result;
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

    if (m_values.size() == 2 && m_mode == dataref_mode::momentary) {
        std::string value;
        if (in_msg.data_2() == MIDI_VELOCITY_MAX)
            value = m_values[0];
        else
            value = m_values[1];

        // change dataref and display label
        in_msg.log().debug(" --> Change dataref '" + m_dataref + "' to value '" + value + "'");

        env().drf().write(in_msg.log(), m_dataref, value);
        display_label(in_msg.log(), value);
    } else {
        toggle_dataref(in_msg.log(), m_dataref, m_values);
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
    std::string map_str = " ====== Dataref ======\n";

    if (!sl().empty())
        map_str.append("Sublayer = '" + std::string(sl()) + "'\n");

    map_str.append("Dataref = '" + m_dataref + "'\n");

    if (m_mode == dataref_mode::toggle)
        map_str.append("Mode = 'toggle'\n");
    else
        map_str.append("Mode = 'momentary'\n");

    map_str.append("Values = [");

    for (auto &value: m_values)
        map_str.append(" '" + value + "'");

    map_str.append("]\n");

    return map_str;
}

} // Namespace xmidictrl