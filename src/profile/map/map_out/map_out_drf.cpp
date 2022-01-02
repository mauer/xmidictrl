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
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_out_drf::map_out_drf(std::shared_ptr<xplane> xp)
    : map_out(std::move(xp))
{
}


/**
 * Destructor
 */
map_out_drf::~map_out_drf()
{
    m_datarefs.clear();
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
 * Set value on
 */
void map_out_drf::set_value_on(std::string_view value_on)
{
    m_value_on = value_on;
}


/**
 * Return value on
 */
std::string_view map_out_drf::value_on() const
{
    return m_value_on;
}


/**
 * Set value off
 */
void map_out_drf::set_value_off(std::string_view value_off)
{
    m_value_off = value_off;
}


/**
 * Return value off
 */
std::string_view map_out_drf::value_off() const
{
    return m_value_off;
}


/**
 * Set velocity on
 */
void map_out_drf::set_velocity_on(int velocity_on)
{
    if (velocity_on >= 0 && velocity_on <= 127)
        m_velocity_on = velocity_on;
    else
        m_velocity_on = 127;
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
    if (velocity_off >= 0 && velocity_off <= 127)
        m_velocity_off = velocity_off;
    else
        m_velocity_off = 0;
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
void map_out_drf::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'drf'" << LOG_END
    map::read_config(settings);

    // read dataref
    if (utils::toml_contains(settings, CFG_KEY_DATAREF)) {
        // check if single value or array
        if (settings[CFG_KEY_DATAREF].is_array())
            set_dataref(utils::toml_read_string_array(settings, CFG_KEY_DATAREF));
        else
            set_dataref(utils::toml_read_string(settings, CFG_KEY_DATAREF));
    }

    // read value on
    set_value_on(utils::toml_read_string(settings, CFG_KEY_VALUE_ON, false));

    // read value off
    set_value_off(utils::toml_read_string(settings, CFG_KEY_VALUE_OFF, false));

    // read velocity on
    set_velocity_on(utils::toml_read_int(settings, CFG_KEY_VELOCITY_ON, false));

    // read velocity off
    set_velocity_off(utils::toml_read_int(settings, CFG_KEY_VELOCITY_OFF, false));
}


/**
 * Check the mapping
 */
bool map_out_drf::check()
{
    if (!map::check())
        return false;

    if (m_datarefs.empty())
        return false;

    if (m_value_on.empty() && m_value_off.empty())
        return false;

    return true;
}


/**
 * Create a MIDI outbound task if required
 */
std::shared_ptr<outbound_task> map_out_drf::execute(const mode_out mode)
{
    bool changed = false;

    bool send_msg = false;
    bool send_on = false;
    int send_off = 0;

    // if one value has been changed, all other values have to be checked
    for (auto &dataref: m_datarefs) {
        // get the current value from X-Plane
        std::string value_current;

        if (!m_xp->datarefs().read(dataref, value_current))
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
        if (!m_value_on.empty()) {
            if (value_current == m_value_on) {
                send_on = true;
                break;
            } else if (value_current == m_value_off || m_value_off.empty()) {
                send_off++;
            }
        } else {
            if (value_current == m_value_off) {
                send_off++;
            } else if (value_current == m_value_on || m_value_on.empty()) {
                send_on = true;
                break;
            }
        }
    }

    if (send_on || send_off == m_datarefs.size()) {
        std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

        task->data_changed = changed;

        task->ch = ch();
        task->cc = cc();

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

    task->ch = ch();
    task->cc = cc();
    task->velocity = 0;

    return task;
}

} // Namespace xmidictrl