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
    map::read_config(settings);

    // read command up
    set_command_up(utils::read_string_parameter(settings, CFG_KEY_COMMAND_UP));

    // read command middle
    set_command_middle(utils::read_string_parameter(settings, CFG_KEY_COMMAND_MIDDLE));

    // read command down
    set_command_down(utils::read_string_parameter(settings, CFG_KEY_COMMAND_DOWN));
}


/**
 * Check the mapping
 */
bool map_in_sld::check()
{
    if (!map::check())
        return false;

    if (m_command_up.empty() && m_command_down.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_sld::execute(midi_message &msg)
{
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

} // Namespace xmidictrl