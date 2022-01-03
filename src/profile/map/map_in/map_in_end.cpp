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

#include "map_in_end.h"

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
map_in_end::map_in_end(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_end::type()
{
    return map_type::encoder_drf;
};


/**
 * Set the dataref
 */
void map_in_end::set_dataref(std::string_view dataref)
{
    m_dataref = dataref;
}


/**
 * Return the dataref
 */
std::string_view map_in_end::dataref() const
{
    return m_dataref;
}


/**
 * Set the modifier for up
 */
void map_in_end::set_modifier_up(float modifier)
{
    m_modifier_up = modifier;
}


/**
 * Return modifier for up
 */
float map_in_end::modifier_up() const
{
    return m_modifier_up;
}


/**
 * Set the modifier for down
 */
void map_in_end::set_modifier_down(float modifier)
{
    m_modifier_down = modifier;
}


/**
 * Return modifier for down
 */
float map_in_end::modifier_down() const
{
    return m_modifier_down;
}


/**
 * Set the modifier for fast up
 */
void map_in_end::set_modifier_fast_up(float modifier)
{
    m_modifier_fast_up = modifier;
}


/**
 * Return modifier for fast up
 */
float map_in_end::modifier_fast_up() const
{
    if (m_modifier_fast_up == 0.0f)
        return m_modifier_up;
    else
        return m_modifier_fast_up;
}


/**
 * Set the modifier for fast down
 */
void map_in_end::set_modifier_fast_down(float modifier)
{
    m_modifier_fast_down = modifier;
}


/**
 * Return modifier for fast down
 */
float map_in_end::modifier_fast_down() const
{
    if (m_modifier_fast_down == 0.0f)
        return m_modifier_down;
    else
        return m_modifier_fast_down;
}


/**
 * Read settings from config
 */
void map_in_end::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'end'" << LOG_END
    map_in::read_config(settings);

    // read dataref
    set_dataref(utils::toml_read_string(settings, CFG_KEY_DATAREF));

    // read modifier up
    set_modifier_up(utils::toml_read_float(settings, CFG_KEY_MODIFIER_UP));

    // read modifier down
    set_modifier_down(utils::toml_read_float(settings, CFG_KEY_MODIFIER_DOWN));

    // read modifier fast up
    set_modifier_fast_up(utils::toml_read_float(settings, CFG_KEY_MODIFIER_FAST_UP, false));

    // read modifier fast down
    set_modifier_fast_down(utils::toml_read_float(settings, CFG_KEY_MODIFIER_FAST_DOWN, false));
}


/**
 * Check the mapping
 */
bool map_in_end::check()
{
    if (!map::check())
        return false;

    if (dataref().empty() && modifier_up() == 0.0f && modifier_down() == 0.0f && modifier_fast_up() == 0.0f
        && modifier_fast_down() == 0.0f)
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
bool map_in_end::execute(midi_message &msg, std::string_view sl_value)
{
    if (!check_sublayer(sl_value))
        return true;

    std::string str_value;
    if (!m_xp->datarefs().read(dataref(), str_value))
        return false;

    float float_value = std::strtof(str_value.data(), nullptr);

    if (msg.velocity < 64) {
        // Down
        if (msg.velocity < 61) {
            LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_fast_down() << "'" << LOG_END
            float_value = float_value + modifier_fast_down();
        } else {
            LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_down() << "'" << LOG_END
            float_value = float_value + modifier_down();
        }
    } else if (msg.velocity > 64) {
        // Up
        if (msg.velocity > 68) {
            LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_fast_up() << "'" << LOG_END
            float_value = float_value + modifier_fast_up();
        } else {
            LOG_DEBUG << " --> Modify dataref '" << dataref() << "' by value '" << modifier_up() << "'" << LOG_END
            float_value = float_value + modifier_up();
        }
    }

    std::stringstream ss;

    ss << float_value;
    str_value.assign(ss.str());

    if (!m_xp->datarefs().write(dataref(), str_value))
        return false;

    return true;
}

} // Namespace xmidictrl