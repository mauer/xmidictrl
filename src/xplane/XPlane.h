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

#ifndef XPLANE_H
#define XPLANE_H

// Standard
#include <memory>

// XMidiCtrl
#include "Commands.h"
#include "Data.h"

namespace XMidiCtrl {

class XPlane {
public:
    XPlane();
    ~XPlane();

    typedef std::shared_ptr<XPlane> ptr;

    XPLMPluginID pluginId();

    std::string_view pluginPath();
    std::string_view xplanePath();

    Commands::ptr commands();
    Data::ptr data();
    
private:
    XPLMPluginID m_pluginId;

    std::string m_xplanePath;
    std::string m_pluginPath;

    Commands::ptr m_commands;
    Data::ptr m_data;
};

} // Namespace XMidiCtrl

#endif // XPLANE_H