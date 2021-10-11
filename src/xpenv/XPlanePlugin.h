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

#ifndef XPLANEPLUGIN_H
#define XPLANEPLUGIN_H

// Standard
#include <map>
#include <memory>
#include <string>
#include <string_view>

// X-Plane SDK 
#include "XPLMPlugin.h"

// X-Plane Environment
#include "Environment.h"
#include "ImGuiFontAtlas.h"
#include "XPlaneWindow.h"

namespace XPEnv {

class XPlanePlugin {
public:
    XPlanePlugin(std::string_view name, std::string_view version);
    ~XPlanePlugin();

    virtual void enablePlugin() = 0;
    virtual void disablePlugin() = 0;

    void showWindow(std::string_view windowId);

    Environment::ptr environment();

private:
    void initialise();

    virtual std::shared_ptr<XPlaneWindow> createWindow(std::string_view windowId) = 0;
    std::shared_ptr<XPlaneWindow> findWindow(std::string_view windowId);
    void addWindow(std::string_view windowId, std::shared_ptr<XPlaneWindow> window);


    // members for the plugin
    XPLMPluginID m_pluginId;

    std::string m_name;
    std::string m_version;

    char m_pluginPath[512]{};
    char m_xplanePath[512]{};

    std::map<std::string_view, std::shared_ptr<XPlaneWindow>> m_windows;
    Environment::ptr m_environment;
};

} // Namespace XPEnv

#endif // XPLANEPLUGIN_H