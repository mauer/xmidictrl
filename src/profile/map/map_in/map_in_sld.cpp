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
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_sld::map_in_sld(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
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
 * Set the dataref
 */
void map_in_sld::set_dataref(std::string_view dataref)
{
    m_dataref = dataref;
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
void map_in_sld::set_value_min(float value)
{
    m_value_min = value;
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
void map_in_sld::set_value_max(float value)
{
    m_value_max = value;
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
void map_in_sld::set_command_up(std::string_view command)
{
    m_command_up = command;
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
void map_in_sld::set_command_middle(std::string_view command)
{
    m_command_middle = command;
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
void map_in_sld::set_command_down(std::string_view command)
{
    m_command_down = command;
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
void map_in_sld::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'sld'" << LOG_END
    map_in::read_config(settings);

    // check if dataref was defined
    if (utils::toml_contains(settings, CFG_KEY_DATAREF, false)) {
        LOG_DEBUG << " --> Use 'dataref' mode for slider mapping" << LOG_END

        // read dataref
        set_dataref(utils::toml_read_string(settings, CFG_KEY_DATAREF));

        // read value min
        set_value_min(utils::toml_read_float(settings, CFG_KEY_VALUE_MIN, false));

        // read value max
        set_value_max(utils::toml_read_float(settings, CFG_KEY_VALUE_MAX, false));
    } else {
        LOG_DEBUG << " --> Use 'command' mode for slider mapping" << LOG_END

        // read command up
        set_command_up(utils::toml_read_string(settings, CFG_KEY_COMMAND_UP));

        // read command middle
        set_command_middle(utils::toml_read_string(settings, CFG_KEY_COMMAND_MIDDLE, false));

        // read command down
        set_command_down(utils::toml_read_string(settings, CFG_KEY_COMMAND_DOWN));
    }
}


/**
 * Check the mapping
 */
bool map_in_sld::check()
{
    if (!map::check())
        return false;

    if (!dataref().empty()) {
        // dataref mode
        if (!m_xp->datarefs().check(dataref()))
            return false;

        if (m_value_min < m_value_max)
            return true;
        else
            return false;
    } else {
        // command mode
        if (m_command_up.empty() && m_command_down.empty())
            return false;
        else
            return true;
    }
}


/**
 * Execute the action in X-Plane
 */
bool map_in_sld::execute(midi_message &msg, std::string_view sl_value)
{
    if (!check_sublayer(sl_value))
        return true;

    if (!dataref().empty()) {
        // dataref mode
        float value = 0.0f;
        if (msg.velocity == 0)
            value = m_value_min;
        else if (msg.velocity == 127)
            value = m_value_max;
        else
            value = ((m_value_max - m_value_min) * (msg.velocity / 127.0f)) + m_value_min;

        LOG_DEBUG << " --> Set dataref '" << m_dataref << "' to value '" << value << "'" << LOG_END
        m_xp->datarefs().write(m_dataref, value);
    } else {
        // command mode
        if (msg.velocity <= 10) {
            LOG_DEBUG << " --> Execute command '" << m_command_down << "'" << LOG_END
            m_xp->cmd().execute(m_command_down);
        } else if (msg.velocity >= 117) {
            LOG_DEBUG << " --> Execute command '" << m_command_up << "'" << LOG_END
            m_xp->cmd().execute(m_command_up);
        } else if (msg.velocity >= 50 && msg.velocity <= 70) {
            if (!m_command_middle.empty()) {
                LOG_DEBUG << " --> Execute command '" << m_command_middle << "'" << LOG_END
                m_xp->cmd().execute(m_command_middle);
            }
        }
    }

    return true;
}

} // Namespace xmidictrl