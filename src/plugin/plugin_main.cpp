//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

// X-Plane SDK
#include "XPLMDefs.h"
#include "XPLMPlugin.h"

// XMidiCtrl
#include "plugin.h"

#ifndef XPLM300
    #error This is made to be compiled against the X-Plane SDK Version 300
#endif


/*
 *  The XPluginStart function is called by X-Plane right after the plugin's DLL is loaded.
 */
PLUGIN_API int XPluginStart(char *out_name, char *out_sig, char *out_desc)
{
#ifdef _MSC_VER
    strcpy_s(out_name, sizeof(XMIDICTRL_NAME), XMIDICTRL_NAME);
    strcpy_s(out_sig, sizeof(XMIDICTRL_SIGNATURE), XMIDICTRL_SIGNATURE);
    strcpy_s(out_desc, sizeof(XMIDICTRL_DESCRIPTION), XMIDICTRL_DESCRIPTION);
#else
    strcpy(out_name, XMIDICTRL_NAME);
    strcpy(out_sig, XMIDICTRL_SIGNATURE);
    strcpy(out_desc, XMIDICTRL_DESCRIPTION);
#endif

    XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

    return 1;
}


/*
 *  The XPluginStop function is called by X-Plane right before the DLL is unloaded.
 *  The plugin will be disabled (if it was enabled) before this routine is called.
 */
PLUGIN_API void XPluginStop(void)
{}


/*
 *  The XPluginEnable function is called by X-Plane right before the plugin is enabled.
 *  Until the plugin is enabled, it will not receive any other callbacks and its UI will be hidden and/or disabled.
 */
PLUGIN_API int XPluginEnable(void)
{
    xmidictrl::plugin::instance().enable();

    return 1;
}


/*
 *  The XPluginDisable function is called by X-Plane right before the plugin is disabled. When the plugin is disabled,
 *  it will not receive any other callbacks and its UI will be hidden and/or disabled.
 */
PLUGIN_API void XPluginDisable(void)
{
    xmidictrl::plugin::instance().disable();
}


/**
 * Process messages from X-Plane
 */
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, int in_msg, void *)
{
    switch (in_msg) {
        case XPLM_MSG_LIVERY_LOADED:
            xmidictrl::plugin::instance().load_profile();
            break;

        case XPLM_MSG_PLANE_UNLOADED:
            xmidictrl::plugin::instance().close_profile();
            break;

        default:
            break;
    }
}
