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

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

// Standard
#include <map>
#include <memory>
#include <string_view>

// X-Plane SDK
#include "XPLMUtilities.h"

namespace xmidictrl {

class commands {
public:
	commands();
    ~commands();

    typedef std::shared_ptr<commands> ptr;

    void begin(std::string_view cmd);
    void end(std::string_view cmd);
    
    void execute(std::string_view cmd);

private:
    XPLMCommandRef find_command_ref(std::string_view cmd);

    std::map<std::string, XPLMCommandRef> m_command_cache;
};

} // Namespace xmidictrl

#endif // _COMMANDS_H_