//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
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
 * Set the dataref
 */
void map_in_drf::set_dataref(std::string_view dataref)
{
    m_dataref = dataref;
}


/**
 * Return the dataref
 */
std::string_view map_in_drf::dataref() const
{
    return m_dataref;
}


/**
 * Set value on
 */
void map_in_drf::set_value_on(std::string_view value_on)
{
    m_value_on = value_on;
}


/**
 * Return value on
 */
std::string_view map_in_drf::value_on() const
{
    return m_value_on;
}


/**
 * Set value off
 */
void map_in_drf::set_value_off(std::string_view value_off)
{
    m_value_off = value_off;
}


/**
 * Return value off
 */
std::string_view map_in_drf::value_off() const
{
    return m_value_off;
}



/**
 * Read settings from config
 */
void map_in_drf::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'drf'" << LOG_END
    map::read_config(settings);

    // read dataref
    set_dataref(utils::read_string_parameter(settings, CFG_KEY_DATAREF));

    // read value on
    set_value_on(utils::read_string_parameter(settings, CFG_KEY_VALUE_ON));

    // read value off
    set_value_off(utils::read_string_parameter(settings, CFG_KEY_VALUE_OFF, false));
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

    if (m_value_on.empty() && m_value_off.empty())
        return false;

    return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_drf::execute(midi_message &msg)
{
    LOG_DEBUG << " --> Change dataref '" << m_dataref << "'" << LOG_END

    m_xp->datarefs().toggle(m_dataref, m_value_on, m_value_off);
}

} // Namespace xmidictrl