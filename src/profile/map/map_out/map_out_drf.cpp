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
 * Set the dataref
 */
void map_out_drf::set_dataref(std::string_view dataref)
{
    m_dataref = dataref;
}


/**
 * Return the dataref
 */
std::string_view map_out_drf::dataref() const
{
    return m_dataref;
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
 * Read settings from config
 */
void map_out_drf::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'drf'" << LOG_END
    map::read_config(settings);

    // read dataref
    set_dataref(utils::read_string_parameter(settings, CFG_KEY_DATAREF));

    // read value on
    set_value_on(utils::read_string_parameter(settings, CFG_KEY_VALUE_ON, false));

    // read value off
    set_value_off(utils::read_string_parameter(settings, CFG_KEY_VALUE_OFF, false));
}


/**
 * Check the mapping
 */
bool map_out_drf::check()
{
    if (!map::check())
        return false;

    if (m_dataref.empty())
        return false;

    if (m_value_on.empty() && m_value_off.empty())
        return false;

    return true;
}


/**
 * Execute the action in X-Plane
 */
std::shared_ptr<midi_message> map_out_drf::execute()
{
    std::string value_now;

    if (!m_xp->datarefs().read(m_dataref, value_now))
        return {};

    if (value_now != m_value_previous) {
        m_value_previous = value_now;

        std::shared_ptr<midi_message> msg = std::make_shared<midi_message>();

        msg->status = OFFSET_MIDI_CHANNEL_STATUS + ch();
        msg->data = cc();

        if (!m_value_on.empty()) {
            if (value_now == m_value_on)
                msg->velocity = 127;
            else if (value_now == m_value_off || m_value_off.empty())
                msg->velocity = 0;
        } else {
            if (value_now == m_value_off)
                msg->velocity = 0;
            else if (value_now == m_value_on || m_value_on.empty())
                msg->velocity = 127;
        }

        return msg;
    }

    return {};
}

} // Namespace xmidictrl