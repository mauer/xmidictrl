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

#include "map_in_enc.h"

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
map_in_enc::map_in_enc(xplane::ptr xp, toml::value &settings)
    : map_in(std::move(xp), settings)
{
    read_config(settings);
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
mapping_type map_in_enc::type()
{
    return mapping_type::encoder;
};


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
 * Check the mapping
 */
bool map_in_enc::check()
{
    if (!map::check())
        return false;

    if (m_command_up.empty() && m_command_down.empty() && m_command_fast_up.empty() && m_command_fast_down.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_enc::execute(midi_message::ptr midi_event)
{
    if (midi_event->velocity < 64) {
        // Down
        if (midi_event->velocity < 61)
            m_xp->cmd().execute(m_command_fast_down);
        else
            m_xp->cmd().execute(m_command_down);
    } else if (midi_event->velocity > 64) {
        // Up
        if (midi_event->velocity > 68)
            m_xp->cmd().execute(m_command_fast_up);
        else
            m_xp->cmd().execute(m_command_up);
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read settings from config
 */
void map_in_enc::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'enc'" << LOG_END

    // read command up
    set_command_up(utils::read_config_parameter(settings, CFG_KEY_COMMAND_UP));

    // read command down
    set_command_down(utils::read_config_parameter(settings, CFG_KEY_COMMAND_DOWN));

    // read fast command up
    set_command_fast_up(utils::read_config_parameter(settings, CFG_KEY_COMMAND_FAST_UP, false));

    // read fast command down
    set_command_fast_down(utils::read_config_parameter(settings, CFG_KEY_COMMAND_FAST_DOWN, false));
}

} // Namespace XMidiCtrl