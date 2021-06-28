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
#include "XPLMDefs.h"
#include "XPLMPlugin.h"

// XMidiCtrl
#include "Plugin.h"
//#include "Global.h"


#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif


/*
 *  The XPluginStart function is called by X-Plane right after the plugin's DLL is loaded.
 */
PLUGIN_API int XPluginStart(char *outName, char *outSig, char *outDesc) {
    strcpy(outName, "XMidiCtrl");
    strcpy(outSig,  "XMidiCtrl");
    strcpy(outDesc, "Midi Controller for X-Plane");

    XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

    return 1;
}


/*
 *  The XPluginStop function is called by X-Plane right before the DLL is unloaded.
 *  The plugin will be disabled (if it was enabled) before this routine is called.
 */
PLUGIN_API void XPluginStop(void) {
}


/*
 *  The XPluginEnable function is called by X-Plane right before the plugin is enabled.
 *  Until the plugin is enabled, it will not receive any other callbacks and its UI will be hidden and/or disabled.
 */
PLUGIN_API int XPluginEnable(void) {
    XMidiCtrl::Plugin::Instance().enablePlugin();
        
    return 1;
}


/*
 *  The XPluginDisable function is called by X-Plane right before the plugin is disabled. When the plugin is disabled,
 *  it will not receive any other callbacks and its UI will be hidden and/or disabled.
 */
PLUGIN_API void XPluginDisable(void) {
    XMidiCtrl::Plugin::Instance().disablePlugin();
}


/**
 * Process messages from X-Plane
 */
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) { 
    switch(inMsg) {
        case XPLM_MSG_PLANE_LOADED:
            XMidiCtrl::Plugin::Instance().reloadSettings();
            break;

        case XPLM_MSG_PLANE_UNLOADED:
            XMidiCtrl::Plugin::Instance().reloadSettings();
            break;
    }
}