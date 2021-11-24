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

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

// Standard
#include <map>

// X-Plane SDK
#include "XPLMProcessing.h"

// XMidiCtrl
#include "menu.h"
#include "profile.h"
#include "settings.h"
#include "types.h"
#include "xplane.h"
#include "XPlaneWindow.h"

namespace xmidictrl {

class plugin {
public:
    plugin();
    ~plugin();

    static plugin &instance();

    static float callback_flight_loop(float elapsed_me, float elapsed_sim, int counter, void *refcon);

    void enable();
    void disable();

    void load_profile();
    void close_profile();

    void show_messages_dialog();
    void show_devices_dialog();
    void show_settings_dialog();
    void show_about_dialog();

private:
    void process_flight_loop(float elapsed_me, float elapsed_sim, int counter);

    void create_window(WindowType windowType);

    XPLMFlightLoopID m_flight_loop_id {nullptr};

    xplane::ptr m_xplane;

    std::unique_ptr<menu> m_menu;

    settings::ptr m_settings;
    std::unique_ptr<profile> m_profile;

    // list of all plugin windows
    std::map<WindowType, XPlaneWindow::ptr> m_windows;
};

} // Namespace XMidiCtrl

#endif // _PLUGIN_H_