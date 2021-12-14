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

#include "map_in_pnp.h"

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
map_in_pnp::map_in_pnp(std::shared_ptr<xplane> xp)
    : map_in(std::move(xp))
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_in_pnp::type()
{
    return map_type::push_pull;
};


/**
 * Set the push command
 */
void map_in_pnp::set_command_push(std::string_view command)
{
    m_command_push = command;
}


/**
 * Return the push command
 */
std::string_view map_in_pnp::command_push() const
{
    return m_command_push;
}


/**
 * Set the pull command
 */
void map_in_pnp::set_command_pull(std::string_view command)
{
    m_command_pull = command;
}


/**
 * Return the pull command
 */
std::string_view map_in_pnp::command_pull() const
{
    return m_command_pull;
}


/**
 * Set the command type
 */
void map_in_pnp::set_command_type(CommandType commandType)
{
    m_command_type = commandType;
}


/**
 * Read settings from config
 */
void map_in_pnp::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'pnp'" << LOG_END
    map::read_config(settings);

    // read command push
    set_command_push(utils::read_string_parameter(settings, CFG_KEY_COMMAND_PUSH));

    // read command pull
    set_command_pull(utils::read_string_parameter(settings, CFG_KEY_COMMAND_PULL));
}


/**
 * Check the mapping
 */
bool map_in_pnp::check()
{
    if (!map::check())
        return false;

    if (m_command_push.empty() && m_command_pull.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_pnp::execute(midi_message &mmsg)
{
    switch (m_command_type) {
        case CommandType::Push:
            LOG_DEBUG << " --> Execute command '" << m_command_push << "'" << LOG_END
            m_xp->cmd().execute(m_command_push);
            break;

        case CommandType::Pull:
            LOG_DEBUG << " --> Execute command '" << m_command_pull << "'" << LOG_END
            m_xp->cmd().execute(m_command_pull);
            break;
    }
}

} // Namespace xmidictrl