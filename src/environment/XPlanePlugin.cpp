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

// X-Plane SDK
#include "XPLMUtilities.h"

// X-Plane Environment
#include "utils/Logger.h"
#include "utils/LogEntry.h"
#include "XPlanePlugin.h"

namespace XPEnv {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
XPlanePlugin::XPlanePlugin(std::string_view name, std::string_view version) {
    // create environment (link to X-Plane data)
    m_environment = std::make_shared<Environment>();

    // initialize the plugin
    initialise();
}


/**
 * Destructor
 */
XPlanePlugin::~XPlanePlugin() = default;




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the X-Plane Environment
 */
Environment::ptr XPlanePlugin::environment() {
    return m_environment;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Initialise the plugin
 */
void XPlanePlugin::initialise() {


    // get plugin path
    //XPLMGetPluginInfo(m_pluginId, nullptr, m_pluginPath, nullptr, nullptr);

    // get X-Plane path
    //XPLMGetSystemPath(m_xplanePath);

    // initialize logger
    //XMidiCtrl::Logger::Instance().initialise(m_xplanePath, m_name);


}

} // Namespace XPEnv