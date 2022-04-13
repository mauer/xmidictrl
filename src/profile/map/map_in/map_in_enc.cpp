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

#include "map_in_enc.h"

// XMidiCtrl
#include "toml_utils.h"
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_enc::map_in_enc(xplane &in_xp)
    : map_in(in_xp)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_enc::type()
{
    return map_type::encoder;
}


/**
 * Read settings from config
 */
void map_in_enc::read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    in_log.debug(" --> Line %i :: Read settings for type 'enc'", in_data.location().line());
    map_in::read_config(in_log, in_data, in_config);

    // read the mode
    m_mode = conversions::encoder_mode_from_code(toml_utils::read_string(in_log, in_data, CFG_KEY_MODE, false));

    // check if dataref was defined
    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF, false)) {
        in_log.debug(" --> Use 'dataref' mode for encoder mapping");

        // read dataref
        m_dataref = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF);

        // read modifier up
        m_modifier_up = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_UP);

        // read modifier down
        m_modifier_down = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_DOWN);

        // read modifier fast up
        if (toml_utils::contains(in_log, in_data, CFG_KEY_MODIFIER_FAST_UP, false))
            m_modifier_fast_up = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_FAST_UP);
        else
            m_modifier_fast_up = m_modifier_up;

        // read modifier fast down
        if (toml_utils::contains(in_log, in_data, CFG_KEY_MODIFIER_FAST_DOWN, false))
            m_modifier_fast_down = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_FAST_DOWN);
        else
            m_modifier_fast_down = m_modifier_down;

        // read value min
        if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_MIN, false)) {
            m_value_min = toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MIN, false);
            m_value_min_defined = true;
        }

        // read value max
        if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_MAX, false)) {
            m_value_max = toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MAX, false);
            m_value_max_defined = true;
        }
    } else {
        in_log.debug(" --> Use 'command' mode for encoder mapping");

        // read command up
        m_command_up = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_UP);

        // read command down
        m_command_down = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_DOWN);

        // read fast command up
        if (toml_utils::contains(in_log, in_data, CFG_KEY_COMMAND_FAST_UP, false))
            m_command_fast_up = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_FAST_UP);
        else
            m_command_fast_up = m_command_up;

        // read fast command down
        if (toml_utils::contains(in_log, in_data, CFG_KEY_COMMAND_FAST_DOWN, false))
            m_command_fast_down = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_FAST_DOWN);
        else
            m_command_fast_down = m_command_down;
    }
}


/**
 * Check the mapping
 */
bool map_in_enc::check(text_logger &in_log)
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

        if (m_modifier_up == 0.0f && m_modifier_down == 0.0f && m_modifier_fast_up == 0.0f
            && m_modifier_fast_down == 0.0f) {
            in_log.error(source_line());
            in_log.error(" --> Modifiers (up/down) are not defined");
            result = false;
        }

        if (m_value_min_defined && m_value_max_defined && m_value_min >= m_value_max) {
            in_log.error(source_line());
            in_log.error(" --> Parameter '%s' is expected to be less than parameter '%s'",
                         CFG_KEY_VALUE_MIN,
                         CFG_KEY_VALUE_MAX);
            result = false;
        }
    } else {
        // command mode
        if (m_command_up.empty() && m_command_down.empty() && m_command_fast_up.empty() && m_command_fast_down.empty()) {
            in_log.error(source_line());
            in_log.error(" --> Commands (up/down) are not defined");
            result = false;
        }
    }

    return result;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_enc::execute(midi_message &in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    if (!m_dataref.empty())
        return execute_dataref(in_msg);
    else
        return execute_command(in_msg);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Execute in dataref mode
 */
bool map_in_enc::execute_dataref(midi_message &in_msg)
{
    float value = 0.0f;

    // read current value
    if (!xp().datarefs().read(in_msg.log(), m_dataref, value)) {
        m_velocity_prev = in_msg.data_2();
        return false;
    }

    if (m_mode == encoder_mode::relative) {
        // relative mode
        if (in_msg.data_2() < 64) {
            // Down
            if (in_msg.data_2() < 61) {
                in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_fast_down);
                value = value + m_modifier_fast_down;
            } else {
                in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_down);
                value = value + m_modifier_down;
            }

            if (m_value_min_defined && value < m_value_min)
                value = m_value_min;
        } else if (in_msg.data_2() > 64) {
            // Up
            if (in_msg.data_2() > 68) {
                in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_fast_up);
                value = value + m_modifier_fast_up;
            } else {
                in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_up);
                value = value + m_modifier_up;
            }

            if (m_value_max_defined && value > m_value_max)
                value = m_value_max;
        }
    } else {
        // range mode
        if (m_velocity_prev == MIDI_NONE) {
            // ignore first message, have to figure out first which way the knob was turned
            m_velocity_prev = in_msg.data_2();
            return true;
        }

        switch (in_msg.data_2()) {
            case MIDI_VELOCITY_MIN:
                in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_up);
                value = value + m_modifier_down;

                if (m_value_min_defined && value < m_value_min)
                    value = m_value_min;

                break;

            case MIDI_VELOCITY_MAX:
                in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_up);
                value = value + m_modifier_up;

                if (m_value_max_defined && value > m_value_max)
                    value = m_value_max;

                break;

            default:
                if ((int) (in_msg.data_2() - m_velocity_prev) > 0) {
                    in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_up);
                    value = value + m_modifier_up;

                    if (m_value_max_defined && value > m_value_max)
                        value = m_value_max;
                } else {
                    in_msg.log().debug(" --> Modify dataref '%s' by value '%f'", m_dataref.data(), m_modifier_down);
                    value = value + m_modifier_down;

                    if (m_value_min_defined && value < m_value_min)
                        value = m_value_min;
                }
                break;
        }
    }

    m_velocity_prev = in_msg.data_2();

    if (!xp().datarefs().write(in_msg.log(), m_dataref, value)) {
        try {
            display_label(std::to_string(value));
        } catch (std::bad_alloc &ex) {
            in_msg.log().error("Error converting float '%f' value to string", value);
            in_msg.log().error(ex.what());
        }

        return false;
    }

    return true;
}


/**
 * Execute in command mode
 */
bool map_in_enc::execute_command(midi_message &in_msg)
{
    if (m_mode == encoder_mode::relative) {
        // relative mode
        if (in_msg.data_2() < 64) {
            // Down
            if (in_msg.data_2() < 61) {
                in_msg.log().debug(" --> Execute command '%s'", m_command_fast_down.data());
                xp().cmd().execute(in_msg.log(), m_command_fast_down);
            } else {
                in_msg.log().debug(" --> Execute command '%s'", m_command_down.data());
                xp().cmd().execute(in_msg.log(), m_command_down);
            }
        } else if (in_msg.data_2() > 64) {
            // Up
            if (in_msg.data_2() > 68) {
                in_msg.log().debug(" --> Execute command '%s'", m_command_fast_up.data());
                xp().cmd().execute(in_msg.log(), m_command_fast_up);
            } else {
                in_msg.log().debug(" --> Execute command '%s'", m_command_up.data());
                xp().cmd().execute(in_msg.log(), m_command_up);
            }
        }
    } else {
        // range mode
        switch (in_msg.data_2()) {
            case MIDI_VELOCITY_MIN:
                in_msg.log().debug(" --> Execute command '%s'", m_command_down.data());
                xp().cmd().execute(in_msg.log(), m_command_down);
                break;

            case MIDI_VELOCITY_MAX:
                in_msg.log().debug(" --> Execute command '%s'", m_command_up.data());
                xp().cmd().execute(in_msg.log(), m_command_up);
                break;

            default:
                if ((int) (in_msg.data_2() - m_velocity_prev) > 0) {
                    in_msg.log().debug(" --> Execute command '%s'", m_command_up.data());
                    xp().cmd().execute(in_msg.log(), m_command_up);
                } else {
                    in_msg.log().debug(" --> Execute command '%s'", m_command_down.data());
                    xp().cmd().execute(in_msg.log(), m_command_down);
                }
                break;
        }

        m_velocity_prev = in_msg.data_2();
    }

    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_enc::build_mapping_text()
{
    std::string map_str = " ====== Encoder ======\n";

    if (m_mode == encoder_mode::relative)
        map_str.append("Mode = 'relative'\n");
    else
        map_str.append("Mode = 'range'\n");

    if (!m_dataref.empty()) {
        map_str.append("Dataref = '" + m_dataref + "'\n");
        map_str.append("Modifier up = '" + std::to_string(m_modifier_up) + "'\n");

        if (m_modifier_fast_up != 0)
            map_str.append("Modifier up (fast) = '" + std::to_string(m_modifier_fast_up) + "'\n");

        map_str.append("Modifier down = '" + std::to_string(m_modifier_down) + "'\n");

        if (m_modifier_fast_down != 0)
            map_str.append("Modifier down (fast) = '" + std::to_string(m_modifier_fast_down) + "'\n");
    } else {
        map_str.append("Command up = '" + m_command_up + "'\n");

        if (!m_command_fast_up.empty())
            map_str.append("Command up (fast) = '" + m_command_fast_up + "'\n");

        map_str.append("Command down = '" + m_command_down + "'\n");

        if (!m_command_fast_down.empty())
            map_str.append("Command down (fast) = '" + m_command_fast_down + "'\n");
    }

    return map_str;
}

} // Namespace xmidictrl