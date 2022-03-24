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
#include "logger.h"
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_enc::map_in_enc(xplane *xp)
    : map_in(xp)
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
};


/*
 * Set the encoder mode
 */
void map_in_enc::set_mode(encoder_mode mode)
{
    m_mode = mode;
}


/**
 * Return the encoder mode
 */
encoder_mode map_in_enc::mode() const
{
    return m_mode;
}


/**
 * Set the dataref
 */
void map_in_enc::set_dataref(std::string_view dataref)
{
    m_dataref = dataref;
}


/**
 * Return the dataref
 */
std::string_view map_in_enc::dataref() const
{
    return m_dataref;
}


/**
 * Set the modifier for up
 */
void map_in_enc::set_modifier_up(float modifier)
{
    m_modifier_up = modifier;
}


/**
 * Return modifier for up
 */
float map_in_enc::modifier_up() const
{
    return m_modifier_up;
}


/**
 * Set the modifier for down
 */
void map_in_enc::set_modifier_down(float modifier)
{
    m_modifier_down = modifier;
}


/**
 * Return modifier for down
 */
float map_in_enc::modifier_down() const
{
    return m_modifier_down;
}


/**
 * Set the modifier for fast up
 */
void map_in_enc::set_modifier_fast_up(float modifier)
{
    m_modifier_fast_up = modifier;
}


/**
 * Return modifier for fast up
 */
float map_in_enc::modifier_fast_up() const
{
    if (m_modifier_fast_up == 0.0f)
        return m_modifier_up;
    else
        return m_modifier_fast_up;
}


/**
 * Set the modifier for fast down
 */
void map_in_enc::set_modifier_fast_down(float modifier)
{
    m_modifier_fast_down = modifier;
}


/**
 * Return modifier for fast down
 */
float map_in_enc::modifier_fast_down() const
{
    if (m_modifier_fast_down == 0.0f)
        return m_modifier_down;
    else
        return m_modifier_fast_down;
}


/**
 * Set the command for up
 */
void map_in_enc::set_command_up(std::string_view command_up)
{
    m_command_up = command_up;
}


/**
 * Return command for up
 */
std::string_view map_in_enc::command_up() const
{
    return m_command_up;
}


/**
 * Set the command for fast up
 */
void map_in_enc::set_command_fast_up(std::string_view command_fast_up)
{
    m_command_fast_up = command_fast_up;
}


/**
 * Return command for fast up
 */
std::string_view map_in_enc::command_fast_up() const
{
    if (m_command_fast_up.empty())
        return m_command_up;
    else
        return m_command_fast_up;
}


/**
 * Set the command for down
 */
void map_in_enc::set_command_down(std::string_view command_down)
{
    m_command_down = command_down;
}


/**
 * Return command for down
 */
std::string_view map_in_enc::command_down() const
{
    return m_command_down;
}


/**
 * Set the command for fast down
 */
void map_in_enc::set_command_fast_down(std::string_view command_fast_down)
{
    m_command_fast_down = command_fast_down;
}


/**
 * Return command for fast down
 */
std::string_view map_in_enc::command_fast_down() const
{
    if (m_command_fast_down.empty())
        return m_command_down;
    else
        return m_command_fast_down;
}


/**
 * Read settings from config
 */
void map_in_enc::read_config(message_handler *messages, toml::value &data)
{
    messages->debug(" --> Line %i :: Read settings for type 'enc'", data.location().line());
    map_in::read_config(messages, data);

    toml_utils utils(messages);

    // read the mode
    set_mode(utils::encoder_mode_from_code(utils.read_string(data, CFG_KEY_MODE, false)));

    // check if dataref was defined
    if (utils.contains(data, CFG_KEY_DATAREF, false)) {
        messages->debug(" --> Use 'dataref' mode for encoder mapping");

        // read dataref
        set_dataref(utils.read_string(data, CFG_KEY_DATAREF));

        // read modifier up
        set_modifier_up(utils.read_float(data, CFG_KEY_MODIFIER_UP));

        // read modifier down
        set_modifier_down(utils.read_float(data, CFG_KEY_MODIFIER_DOWN));

        // read modifier fast up
        set_modifier_fast_up(utils.read_float(data, CFG_KEY_MODIFIER_FAST_UP, false));

        // read modifier fast down
        set_modifier_fast_down(utils.read_float(data, CFG_KEY_MODIFIER_FAST_DOWN, false));

        // read value min
        if (utils.contains(data, CFG_KEY_VALUE_MIN)) {
            m_value_min = utils.read_float(data, CFG_KEY_VALUE_MIN, false);
            m_value_min_defined = true;
        }

        // read value max
        if (utils.contains(data, CFG_KEY_VALUE_MAX)) {
            m_value_max = utils.read_float(data, CFG_KEY_VALUE_MAX, false);
            m_value_max_defined = true;
        }
    } else {
        messages->debug(" --> Use 'command' mode for encoder mapping");

        // read command up
        set_command_up(utils.read_string(data, CFG_KEY_COMMAND_UP));

        // read command down
        set_command_down(utils.read_string(data, CFG_KEY_COMMAND_DOWN));

        // read fast command up
        set_command_fast_up(utils.read_string(data, CFG_KEY_COMMAND_FAST_UP, false));

        // read fast command down
        set_command_fast_down(utils.read_string(data, CFG_KEY_COMMAND_FAST_DOWN, false));
    }
}


/**
 * Check the mapping
 */
bool map_in_enc::check()
{
    if (!map::check())
        return false;

    if (!dataref().empty()) {
        // dataref mode
        if (!m_xp->datarefs().check(dataref()))
            return false;

        if (modifier_up() == 0.0f && modifier_down() == 0.0f && modifier_fast_up() == 0.0f
            && modifier_fast_down() == 0.0f)
            return false;

        if (m_value_min_defined && m_value_max_defined && m_value_min >= m_value_max)
            return false;

        return true;
    } else {
        // command mode
        if (command_up().empty() && command_down().empty() && command_fast_up().empty() && command_fast_down().empty())
            return false;
        else
            return true;
    }
}


/**
 * Execute the action in X-Plane
 */
bool map_in_enc::execute(message_handler *messages, midi_message &msg, std::string_view sl_value)
{
    if (!check_sublayer(sl_value))
        return true;

    if (!dataref().empty())
        return execute_dataref(msg);
    else
        return execute_command(msg);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Execute in dataref mode
 */
bool map_in_enc::execute_dataref(midi_message &msg)
{
    float value = 0.0f;

    // read current value
    if (!m_xp->datarefs().read(dataref(), value)) {
        m_velocity_prev = msg.data_2;
        return false;
    }

    if (m_mode == encoder_mode::relative) {
        // relative mode
        if (msg.data_2 < 64) {
            // Down
            if (msg.data_2 < 61) {
                LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_fast_down() << "'"
                          << LOG_END

                value = value + modifier_fast_down();
            } else {
                LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_down() << "'"
                          << LOG_END

                value = value + modifier_down();
            }

            if (m_value_min_defined && value < m_value_min)
                value = m_value_min;
        } else if (msg.data_2 > 64) {
            // Up
            if (msg.data_2 > 68) {
                LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_fast_up() << "'"
                          << LOG_END

                value = value + modifier_fast_up();
            } else {
                LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_up() << "'"
                          << LOG_END

                value = value + modifier_up();
            }

            if (m_value_max_defined && value > m_value_max)
                value = m_value_max;
        }
    } else {
        // range mode
        if (m_velocity_prev == MIDI_NONE) {
            // ignore first message, have to figure out first which way the knob was turned
            m_velocity_prev = msg.data_2;
            return true;
        }

        switch (msg.data_2) {
            case MIDI_VELOCITY_MIN:
                LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_down() << "'" << LOG_END
                value = value + modifier_down();

                if (m_value_min_defined && value < m_value_min)
                    value = m_value_min;

                break;

            case MIDI_VELOCITY_MAX:
                LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_up() << "'" << LOG_END
                value = value + modifier_up();

                if (m_value_max_defined && value > m_value_max)
                    value = m_value_max;

                break;

            default:
                if ((int) (msg.data_2 - m_velocity_prev) > 0) {
                    LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_up() << "'"
                              << LOG_END
                    value = value + modifier_up();

                    if (m_value_max_defined && value > m_value_max)
                        value = m_value_max;
                } else {
                    LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_down() << "'"
                              << LOG_END
                    value = value + modifier_down();

                    if (m_value_min_defined && value < m_value_min)
                        value = m_value_min;
                }
                break;
        }
    }

    m_velocity_prev = msg.data_2;

    if (!m_xp->datarefs().write(dataref(), value))
        return false;

    return true;
}


/**
 * Execute in command mode
 */
bool map_in_enc::execute_command(midi_message &msg)
{
    if (m_mode == encoder_mode::relative) {
        // relative mode
        if (msg.data_2 < 64) {
            // Down
            if (msg.data_2 < 61) {
                LOG_DEBUG << " --> Execute command '" << command_fast_down() << "'" << LOG_END
                m_xp->cmd().execute(command_fast_down());
            } else {
                LOG_DEBUG << " --> Execute command '" << command_down() << "'" << LOG_END
                m_xp->cmd().execute(command_down());
            }
        } else if (msg.data_2 > 64) {
            // Up
            if (msg.data_2 > 68) {
                LOG_DEBUG << " --> Execute command '" << command_fast_up() << "'" << LOG_END
                m_xp->cmd().execute(command_fast_up());
            } else {
                LOG_DEBUG << " --> Execute command '" << command_up() << "'" << LOG_END
                m_xp->cmd().execute(command_up());
            }
        }
    } else {
        // range mode
        switch (msg.data_2) {
            case MIDI_VELOCITY_MIN:
                LOG_DEBUG << " --> Execute command '" << command_down() << "'" << LOG_END
                m_xp->cmd().execute(command_down());
                break;

            case MIDI_VELOCITY_MAX:
                LOG_DEBUG << " --> Execute command '" << command_up() << "'" << LOG_END
                m_xp->cmd().execute(command_up());
                break;

            default:
                if ((int) (msg.data_2 - m_velocity_prev) > 0) {
                    LOG_DEBUG << " --> Execute command '" << command_up() << "'" << LOG_END
                    m_xp->cmd().execute(command_up());
                } else {
                    LOG_DEBUG << " --> Execute command '" << command_down() << "'" << LOG_END
                    m_xp->cmd().execute(command_down());
                }
                break;
        }

        m_velocity_prev = msg.data_2;
    }

    return true;
}

} // Namespace xmidictrl