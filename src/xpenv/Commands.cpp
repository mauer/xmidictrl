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

// X-Plane Environment
#include "Commands.h"
#include "PluginLogger.h"


namespace XPEnv {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Commands::Commands() {

}


/**
 * Destructor
 */
Commands::~Commands() {

}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Begin a X-Plane command
 */
void Commands::begin(const std::string& command) {
    XPLMCommandRef cmdRef = getCommandRef(command);

    if (cmdRef != nullptr)
        XPLMCommandBegin(cmdRef);
}


/**
 * End a X-Plane command
 */
void Commands::end(const std::string& command) {
    XPLMCommandRef cmdRef = getCommandRef(command);

    if (cmdRef != nullptr)
        XPLMCommandEnd(cmdRef);
}


/**
 * Execute a X-Plane command
 */
void Commands::execute(const std::string& command) {
    XPLMCommandRef cmdRef = getCommandRef(command);

    if (cmdRef != nullptr)
        XPLMCommandOnce(cmdRef);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the command ref for a command string
 */
XPLMCommandRef Commands::getCommandRef(const std::string& command) {
    XPLMCommandRef cmdRef = nullptr;

    // check the cache first
    try {
        cmdRef = m_commandCache.at(command);
    } catch (std::out_of_range const&) {
        cmdRef = XPLMFindCommand(command.c_str());
        m_commandCache.emplace(command, cmdRef);
    }

    if (cmdRef == nullptr)
        LOG_ERROR << "Command '" << command << "' not found" << LOG_END

    return cmdRef;
}

} // XPEnv