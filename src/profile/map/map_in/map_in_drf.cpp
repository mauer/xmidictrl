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
#include "logger.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_drf::map_in_drf(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
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
};


/**
 * Read settings from config
 */
void map_in_drf::read_config(toml::value &settings)
{
    LOG_DEBUG << " --> Line " << settings.location().line() << " :: Read settings for type 'drf'" << LOG_END
    map_in::read_config(settings);

    // read mode
    m_mode = utils::dataref_mode_from_code(utils::toml_read_string(settings, CFG_KEY_MODE, false));

    // read dataref
    m_dataref = utils::toml_read_string(settings, CFG_KEY_DATAREF);

    // check if a values array has been defined
    m_values = utils::toml_read_str_vector_array(settings, CFG_KEY_VALUES, false);

    if (m_values.empty()) {
        // read value on
        std::string value = utils::toml_read_string(settings, CFG_KEY_VALUE_ON);

        if (!value.empty())
            m_values.push_back(value);

        // read value off
        if (utils::toml_contains(settings, CFG_KEY_VALUE_OFF, false))
            value = utils::toml_read_string(settings, CFG_KEY_VALUE_OFF, false);

        if (!value.empty())
            m_values.push_back(value);
    }
}


/**
 * Check the mapping
 */
bool map_in_drf::check()
{
    if (!map::check())
        return false;

    if (m_dataref.empty())
        return false;

    if (!m_xp->datarefs().check(m_dataref))
        return false;

    if (m_values.empty())
        return false;

    if (m_mode == dataref_mode::momentary && m_values.size() != 2)
        return false;

    return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_drf::execute(midi_message &msg, std::string_view sl_value)
{
    if (!check_sublayer(sl_value))
        return true;

    // if toggle mode then only process key pressed event
    if (m_mode == dataref_mode::toggle && msg.velocity != MIDI_VELOCITY_MAX)
        return true;

    LOG_DEBUG << " --> Change dataref '" << m_dataref << "'" << LOG_END

    if (m_values.size() == 2) {
        if (m_mode == dataref_mode::momentary) {
            if (msg.velocity == MIDI_VELOCITY_MAX)
                m_xp->datarefs().write(m_dataref, m_values[0]);
            else
                m_xp->datarefs().write(m_dataref, m_values[1]);
        } else {
            m_xp->datarefs().toggle(m_dataref, m_values[0], m_values[1]);
        }
    } else {
        // get current value
        std::string value;
        m_xp->datarefs().read(m_dataref, value);

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

        m_xp->datarefs().write(m_dataref, value);
    }

    return true;
}

} // Namespace xmidictrl