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

#include "map_in_enc.h"

// fmt
#include "fmt/format.h"

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
map_in_enc::map_in_enc(environment& in_env, encoder_mode in_default_enc_mode)
    : map_in(in_env),
      m_enc_mode(in_default_enc_mode)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_enc::type()
{
    return map_in_type::encoder;
}


/**
 * Read settings from config
 */
void map_in_enc::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'enc'");
    map_in::read_config(in_log, in_data, in_config);

    // read the mode
    if (toml_utils::contains(in_log, in_data, CFG_KEY_MODE))
        m_enc_mode = conversions::encoder_mode_from_code(toml_utils::read_string(in_log, in_data, CFG_KEY_MODE));

    // read the delay (if defined)
    if (toml_utils::contains(in_log, in_data, CFG_KEY_DELAY))
        m_delay = toml_utils::read_int(in_log, in_data, CFG_KEY_DELAY);

    // check if dataref was defined
    if (toml_utils::contains(in_log, in_data, CFG_KEY_DATAREF)) {
        in_log.debug_line(in_data.location().line(), "Use 'dataref' mode for encoder mapping");
        m_enc_map_type = encoder_map_type::dataref;

        // read dataref
        m_dataref = toml_utils::read_string(in_log, in_data, CFG_KEY_DATAREF);

        // read modifier up
        m_modifier_up = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_UP);

        // read modifier down
        m_modifier_down = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_DOWN);

        // read modifier fast up
        if (toml_utils::contains(in_log, in_data, CFG_KEY_MODIFIER_FAST_UP))
            m_modifier_fast_up = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_FAST_UP);
        else
            m_modifier_fast_up = m_modifier_up;

        // read modifier fast down
        if (toml_utils::contains(in_log, in_data, CFG_KEY_MODIFIER_FAST_DOWN))
            m_modifier_fast_down = toml_utils::read_float(in_log, in_data, CFG_KEY_MODIFIER_FAST_DOWN);
        else
            m_modifier_fast_down = m_modifier_down;

        // read value min
        if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_MIN)) {
            m_value_min = toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MIN, false);
            m_value_min_defined = true;
        }

        // read value max
        if (toml_utils::contains(in_log, in_data, CFG_KEY_VALUE_MAX)) {
            m_value_max = toml_utils::read_float(in_log, in_data, CFG_KEY_VALUE_MAX, false);
            m_value_max_defined = true;
        }
    } else {
        in_log.debug_line(in_data.location().line(), "Use 'command' mode for encoder mapping");
        m_enc_map_type = encoder_map_type::command;

        // read command up
        m_command_up = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_UP);

        // read command down
        m_command_down = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_DOWN);

        // read fast command up
        if (toml_utils::contains(in_log, in_data, CFG_KEY_COMMAND_FAST_UP))
            m_command_fast_up = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_FAST_UP);
        else
            m_command_fast_up = m_command_up;

        // read fast command down
        if (toml_utils::contains(in_log, in_data, CFG_KEY_COMMAND_FAST_DOWN))
            m_command_fast_down = toml_utils::read_string(in_log, in_data, CFG_KEY_COMMAND_FAST_DOWN);
        else
            m_command_fast_down = m_command_down;
    }

    // read data 2 depending on the encoder mode
    switch (m_enc_mode) {
        using enum encoder_mode;

        case relative:
            // TODO: Add data_2 up/down and up fast/down fast
            break;

        case range:
            m_data_2_min = toml_utils::read_unsigned_char(in_log, in_data, c_cfg_data_2_min, MIDI_DATA_2_MIN);
            m_data_2_max = toml_utils::read_unsigned_char(in_log, in_data, c_cfg_data_2_max, MIDI_DATA_2_MAX);
            break;

        case fixed:
            m_data_2_up = toml_utils::read_unsigned_char(in_log, in_data, c_cfg_data_2_up);
            m_data_2_down = toml_utils::read_unsigned_char(in_log, in_data, c_cfg_data_2_down);
            break;
    }
}

/**
 * Check the mapping
 */
bool map_in_enc::check(text_logger& in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    switch (m_enc_map_type) {
        using enum encoder_map_type;

        case dataref:
            // dataref mode
            if (!env().drf().check(m_dataref)) {
                in_log.error(source_line());
                in_log.error(fmt::format(" --> Dataref '{}' not found", m_dataref));
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
                in_log.error(
                    " --> Parameter '" + std::string(CFG_KEY_VALUE_MIN) + "' is expected to be less than parameter '"
                    + std::string(CFG_KEY_VALUE_MAX) + "'");
                result = false;
            }
            break;

        case command:
            // command mode
            if (m_command_up.empty() && m_command_down.empty() && m_command_fast_up.empty()
                && m_command_fast_down.empty()) {
                in_log.error(source_line());
                in_log.error(" --> Commands (up/down) are not defined");
                result = false;
            }
            break;
    }

    // check data 2 depending on the encoder mode
    switch (m_enc_mode) {
        using enum encoder_mode;

        case relative:
            break;

        case range:
            if (m_data_2_min >= m_data_2_max) {
                in_log.error(source_line());
                in_log.error(fmt::format(" --> Parameter '{}' needs to be lower than parameter '{}'", m_data_2_min, m_data_2_max));
                result = false;
            }
            break;

        case fixed:
            if (m_data_2_up == m_data_2_down) {
                in_log.error(source_line());
                in_log.error(fmt::format(" --> Parameter '{}' needs to be different than parameter '{}'", m_data_2_up, m_data_2_down));
                result = false;
            }
            break;
    }

    return result;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_enc::execute(midi_message& in_msg, std::string_view in_sl_value)
{
    if (!check_sublayer(in_sl_value))
        return true;

    if (m_delay > -1) {
        if (m_delay_counter < m_delay) {
            m_delay_counter++;
            return true;
        } else {
            m_delay_counter = 0;
        }
    }

    switch (m_enc_mode) {
        using enum encoder_mode;

        case relative:
            // relative mode
            if (in_msg.data_2() < 64) {
                if (in_msg.data_2() < 61)
                    execute_down(in_msg, true);
                else
                    execute_down(in_msg, false);
            } else if (in_msg.data_2() > 64) {
                // Up
                if (in_msg.data_2() > 68)
                    execute_up(in_msg, true);
                else
                    execute_up(in_msg, false);
            }
            break;

        case range:
            // range mode
            if (!m_data_2_prev_set) {
                // ignore the first message, because we don't know which direction the knob was turned
                m_data_2_prev_set = true;
                return true;
            }

            if (in_msg.data_2() == m_data_2_min) {
                execute_down(in_msg, false);
            } else if (in_msg.data_2() == m_data_2_max) {
                execute_up(in_msg, false);
            } else {
                if ((int) (in_msg.data_2() - m_data_2_prev) > 0)
                    execute_up(in_msg, false);
                else
                    execute_down(in_msg, false);
            }

            m_data_2_prev = in_msg.data_2();
            break;

        case fixed:
            if (in_msg.data_2() == m_data_2_down)
                execute_down(in_msg, false);
            else if (in_msg.data_2() == m_data_2_up)
                execute_up(in_msg, false);

            break;
    }
}


/**
 * Return mapped commands/dataref
 */
std::string map_in_enc::map_text_cmd_drf()
{
    std::string map_str {};

    if (!m_dataref.empty()) {
        map_str.append(m_dataref);
    } else {
        map_str.append(m_command_up + "   (up)");
        map_str.append("\n" + m_command_down + "   (down)");

        if (!m_command_fast_up.empty() && m_command_up != m_command_fast_up)
            map_str.append("\n" + m_command_fast_up + "   (fast up)");

        if (!m_command_fast_down.empty() && m_command_down != m_command_fast_down)
            map_str.append("\n" + m_command_fast_down + "   (fast down)");
    }

    return map_str;
}


/**
 * Return mapped parameter
 */
std::string map_in_enc::map_text_parameter()
{
    std::string map_str {};

    if (!m_dataref.empty()) {
        map_str.append("Modifier up = " + conversions::float_to_string(m_modifier_up));
        map_str.append("   |   ");

        if (m_modifier_fast_up != 0) {
            map_str.append("Modifier up (fast) = " + conversions::float_to_string(m_modifier_fast_up));
            map_str.append("   |   ");
        }

        map_str.append("Modifier down = " + conversions::float_to_string(m_modifier_down));
        map_str.append("   |   ");

        if (m_modifier_fast_down != 0) {
            map_str.append("Modifier down (fast) = " + conversions::float_to_string(m_modifier_fast_down));
            map_str.append("   |   ");
        }
    }

    if (m_enc_mode == encoder_mode::relative)
        map_str.append("Mode = relative");
    else
        map_str.append("Mode = range");

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_enc::build_mapping_text(bool in_short)
{
    std::string map_str {};
    std::string sep_str {", "};

    if (!in_short) {
        sep_str = "\n";
        map_str = " ====== Encoder ======" + sep_str;

        if (!sl().empty())
            map_str.append("Sublayer = '" + std::string(sl()) + "'" + sep_str);
    }

    if (!m_dataref.empty()) {
        map_str.append("Dataref = '" + m_dataref + "'" + sep_str);
        map_str.append("Modifier up = " + conversions::float_to_string(m_modifier_up));

        if (m_modifier_fast_up != 0)
            map_str.append(sep_str + "Modifier up (fast) = " + conversions::float_to_string(m_modifier_fast_up));

        map_str.append(sep_str + "Modifier down = " + conversions::float_to_string(m_modifier_down));

        if (m_modifier_fast_down != 0)
            map_str.append(sep_str + "Modifier down (fast) = " + conversions::float_to_string(m_modifier_fast_down));
    } else {
        map_str.append("Command up = '" + m_command_up + "'" + sep_str);

        if (!m_command_fast_up.empty())
            map_str.append("Command up (fast) = '" + m_command_fast_up + "'" + sep_str);

        map_str.append("Command down = '" + m_command_down + "'");

        if (!m_command_fast_down.empty())
            map_str.append(sep_str + "Command down (fast) = '" + m_command_fast_down + "'");
    }

    if (m_enc_mode == encoder_mode::relative)
        map_str.append(sep_str + "Mode = 'relative'");
    else
        map_str.append(sep_str + "Mode = 'range'");

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Execute in dataref mode
 *//*

bool map_in_enc::execute_dataref(midi_message& in_msg)
{
    float value = 0.0f;
    float modifier = 0.0f;

    // read current value
    if (!env().drf().read(in_msg.log(), m_dataref, value)) {
        m_data_2_prev_set = true;
        m_data_2_prev = in_msg.data_2();
        return true;
    }

    if (m_enc_mode == encoder_mode::relative) {
        // relative mode
        if (in_msg.data_2() < 64) {
            // Down
            if (in_msg.data_2() < 61) {
                in_msg.log().debug(
                    " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_fast_down) + "'");
                modifier = m_modifier_fast_down;
            } else {
                in_msg.log().debug(
                    " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_down) + "'");
                modifier = m_modifier_down;
            }
        } else if (in_msg.data_2() > 64) {
            // Up
            if (in_msg.data_2() > 68) {
                in_msg.log().debug(
                    " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_fast_up) + "'");
                modifier = m_modifier_fast_up;
            } else {
                in_msg.log().debug(
                    " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_up) + "'");
                modifier = m_modifier_up;
            }
        }
    } else {
        // range mode
        if (!m_data_2_prev_set) {
            // ignore first message, have to figure out first which way the knob was turned
            m_data_2_prev_set = true;
            m_data_2_prev = in_msg.data_2();
            return true;
        }

        switch (in_msg.data_2()) {
            // TODO: Make that configurable
            case MIDI_DATA_2_MIN:
                in_msg.log().debug(
                    " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_up) + "'");
                modifier = m_modifier_down;

                break;

                // TODO: Make that configurable
            case MIDI_DATA_2_MAX:
                in_msg.log().debug(
                    " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_up) + "'");
                modifier = m_modifier_up;

                break;

            default:
                if ((int) (in_msg.data_2() - m_data_2_prev) > 0) {
                    in_msg.log().debug(
                        " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_up) + "'");
                    modifier = m_modifier_up;
                } else {
                    in_msg.log().debug(
                        " --> Modify dataref '" + m_dataref + "' by value '" + std::to_string(m_modifier_down) + "'");
                    modifier = m_modifier_down;
                }

                break;
        }
    }

    m_data_2_prev = in_msg.data_2();

    // change and check the value
    value = value + modifier;
    value = check_value_min_max(value, modifier);

    if (env().drf().write(in_msg.log(), m_dataref, value)) {
        try {
            display_label(in_msg.log(), value);
        } catch (std::bad_alloc& ex) {
            in_msg.log().error("Error converting float '" + std::to_string(value) + "' value to string");
            in_msg.log().error(ex.what());
        }
    } else {
        in_msg.log().error("Error changing dataref '" + m_dataref + "' to value '" + std::to_string(value) + "'");
    }

    return true;
}
*/


/**
 * Execute in command mode
 */
/*bool map_in_enc::execute_command(midi_message& in_msg)
{
    if (m_enc_mode == encoder_mode::relative) {
        // relative mode
        if (in_msg.data_2() < 64) {
            // Down
            if (in_msg.data_2() < 61) {
                in_msg.log().debug(" --> Execute command '" + m_command_fast_down + "'");
                env().cmd().execute(in_msg.log(), m_command_fast_down);
            } else {
                in_msg.log().debug(" --> Execute command '" + m_command_down + "'");
                env().cmd().execute(in_msg.log(), m_command_down);
            }
        } else if (in_msg.data_2() > 64) {
            // Up
            if (in_msg.data_2() > 68) {
                in_msg.log().debug(" --> Execute command '" + m_command_fast_up + "'");
                env().cmd().execute(in_msg.log(), m_command_fast_up);
            } else {
                in_msg.log().debug(" --> Execute command '" + m_command_up + "'");
                env().cmd().execute(in_msg.log(), m_command_up);
            }
        }
    } else {
        // range mode
        switch (in_msg.data_2()) {
            case MIDI_DATA_2_MIN:
                in_msg.log().debug(" --> Execute command '" + m_command_down + "'");
                env().cmd().execute(in_msg.log(), m_command_down);
                break;

            case MIDI_DATA_2_MAX:
                in_msg.log().debug(" --> Execute command '" + m_command_up + "'");
                env().cmd().execute(in_msg.log(), m_command_up);
                break;

            default:
                if ((int) (in_msg.data_2() - m_data_2_prev) > 0) {
                    in_msg.log().debug(" --> Execute command '" + m_command_up + "'");
                    env().cmd().execute(in_msg.log(), m_command_up);
                } else {
                    in_msg.log().debug(" --> Execute command '" + m_command_down + "'");
                    env().cmd().execute(in_msg.log(), m_command_down);
                }
                break;
        }

        m_data_2_prev = in_msg.data_2();
    }

    return true;
}*/


/**
 * Execute the up command/modifier
 */
void map_in_enc::execute_up(midi_message& in_msg, bool in_fast)
{
    switch (m_enc_map_type) {
        using enum encoder_map_type;

        case dataref: {
            float value = 0.0f;

            // read current value
            if (!env().drf().read(in_msg.log(), m_dataref, value))
                return;

            float modifier = m_modifier_up;

            if (in_fast)
                modifier = m_modifier_fast_up;

            in_msg.log().debug(fmt::format(" --> Modify dataref '{}' by value '{}'", m_dataref, modifier));

            // change and check the value
            value = value + modifier;
            value = check_value_min_max(value, modifier);

            if (env().drf().write(in_msg.log(), m_dataref, value)) {
                try {
                    display_label(in_msg.log(), value);
                } catch (std::bad_alloc& ex) {
                    in_msg.log().error(fmt::format("Error converting float '{}' value to string", value));
                    in_msg.log().error(ex.what());
                }
            } else {
                in_msg.log().error(fmt::format("Error changing dataref '{}' to value '{}'", m_dataref, value));
            }
        } break;

        case command:
            std::string cmd = m_command_down;

            if (in_fast)
                cmd = m_command_fast_down;

            in_msg.log().debug(" --> Execute command '" + cmd + "'");
            env().cmd().execute(in_msg.log(), cmd);

            break;
    }
}


/**
 * Execute the down command/modifier
 */
void map_in_enc::execute_down(midi_message& in_msg, bool in_fast)
{
    switch (m_enc_map_type) {
        using enum encoder_map_type;

        case dataref:

        case command:
            std::string cmd = m_command_down;

            if (in_fast)
                cmd = m_command_fast_down;

            in_msg.log().debug(" --> Execute command '" + cmd + "'");
            env().cmd().execute(in_msg.log(), cmd);

            break;
    }
}


/**
 * Check if the value if within the min/max range
 */
float map_in_enc::check_value_min_max(float in_value, float in_modifier) const
{
    if (in_modifier < 0) {
        // modifier is negative, so we use the minimum value
        if (m_value_min_defined && in_value < m_value_min)
            return m_value_min;
    } else {
        // modifier is positive, so we have to use the maximum value
        if (m_value_max_defined && in_value > m_value_max)
            return m_value_max;
    }

    return in_value;
}

}// Namespace xmidictrl
