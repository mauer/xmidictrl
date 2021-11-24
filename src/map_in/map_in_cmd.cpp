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

#include "map_in_cmd.h"

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
map_in_cmd::map_in_cmd(xplane::ptr xp, toml::value &settings)
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
mapping_type map_in_cmd::type()
{
    return mapping_type::command;
};


/**
 * Set the command
 */
void map_in_cmd::set_command(std::string_view command)
{
    m_command = command;
}


/**
 * Return the command
 */
std::string_view map_in_cmd::command() const
{
    return m_command;
}


/**
 * Check the mapping
 */
bool map_in_cmd::check()
{
    if (!map::check())
        return false;

    if (m_command.empty())
        return false;
    else
        return true;
}


/**
 * Execute the action in X-Plane
 */
void map_in_cmd::execute(midi_message::ptr midi_event)
{
    LOG_DEBUG << "Execute command '" << m_command << "'" << LOG_END

    switch (midi_event->velocity) {
        case 127:
            m_xp->cmd().begin(m_command);
            break;

        case 0:
            m_xp->cmd().end(m_command);
            break;

        default:
            LOG_ERROR << "Invalid midi velocity '" << midi_event->velocity << "'" << LOG_END
            LOG_ERROR << " --> Supported values are '127' and '0'" << LOG_END
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read settings from config
 */
void map_in_cmd::read_config(toml::value &settings)
{
    LOG_DEBUG << "Line " << settings.location().line() << " :: Read settings for type 'cmd'" << LOG_END

    // read the command
    set_command(utils::read_config_parameter(settings, CFG_KEY_COMMAND));
}

} // Namespace XMidiCtrl