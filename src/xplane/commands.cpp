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

#include "commands.h"

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
commands::commands() = default;


/**
 * Destructor
 */
commands::~commands()
{
    m_command_cache.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Begin a X-Plane command
 */
void commands::begin(std::string_view cmd)
{
    XPLMCommandRef cmd_ref = find_command_ref(cmd);

    if (cmd_ref != nullptr)
        XPLMCommandBegin(cmd_ref);
}


/**
 * End a X-Plane command
 */
void commands::end(std::string_view cmd)
{
    XPLMCommandRef cmd_ref = find_command_ref(cmd);

    if (cmd_ref != nullptr)
        XPLMCommandEnd(cmd_ref);
}


/**
 * Execute a X-Plane command
 */
void commands::execute(std::string_view cmd)
{
    XPLMCommandRef cmd_ref = find_command_ref(cmd);

    if (cmd_ref != nullptr)
        XPLMCommandOnce(cmd_ref);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the command ref for a command string
 */
XPLMCommandRef commands::find_command_ref(std::string_view cmd)
{
    XPLMCommandRef cmd_ref = nullptr;

    // check the cache first
    try {
        cmd_ref = m_command_cache.at(cmd.data());
    } catch (std::out_of_range const &) {
        cmd_ref = XPLMFindCommand(cmd.data());
        m_command_cache.emplace(cmd, cmd_ref);
    }

    if (cmd_ref == nullptr)
        LOG_ERROR << "Command '" << cmd.data() << "' not found" << LOG_END

    return cmd_ref;
}

} // Namespace xmidictrl