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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// Standard
#include <map>
#include <memory>
#include <queue>

// X-Plane SDK
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

// X-Plane Environment
#include "Environment.h"

// XMidiCtrl
#include "MappedEvent.h"
#include "Global.h"

using namespace XPEnv;

namespace XMidiCtrl {

class EventHandler {
public:
    explicit EventHandler(Environment::ptr environment);
    ~EventHandler();
   
    void addMappedEvent(const MappedEvent::ptr& mappedEvent);
    void processEvents();

private:
    std::queue<MappedEvent::ptr> m_eventList;
    std::shared_ptr<Environment> m_environment;
};

} // Namespace XMidiCtrl

#endif // EVENTHANDLER_H