//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
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

#ifndef PLUGIN_H
#define PLUGIN_H

// Standard
#include <map>

// X-Plane SDK
#include "XPLMProcessing.h"

// XMidiCtrl
#include "env_xplane.h"
#include "inbound_worker.h"
#include "info_msg.h"
#include "menu.h"
#include "text_logger.h"
#include "midi_logger.h"
#include "profile.h"
#include "settings.h"
#include "types.h"
#include "xplane_window.h"

namespace xmidictrl {

class plugin {
public:
    plugin();
    ~plugin();

    static plugin &instance();

    static float callback_flight_loop(float in_elapsed_me, float in_elapsed_sim, int in_counter, void *in_refcon);

    void enable();
    void disable();

    void load_profile();
    void close_profile();

    void show_info_message(std::string_view in_id, std::string_view in_msg, int in_seconds = -1);

    void add_inbound_task(const std::shared_ptr<inbound_task> &in_task);

    void show_messages_window();
    void show_devices_window();
    void show_profile_window();
    void show_settings_window();
    void show_about_window();

    [[nodiscard]] int sublayer() const;
    void set_sublayer(int in_value);

    void toggle_sublayer();

private:
    void process_flight_loop([[maybe_unused]] float in_elapsed_me, float in_elapsed_sim, int in_counter);
    void process_info_messages();

    void create_datarefs();
    void remove_datarefs();

    void create_commands();
    void remove_commands();

    static int read_drf_sublayer(void *in_refcon);
    static void write_drf_sublayer(void *in_refcon, int in_value);

    static int command_handler(XPLMCommandRef in_command, XPLMCommandPhase in_phase, void *in_refcon);

    std::shared_ptr<xplane_window> create_window(window_type in_type);

    XPLMFlightLoopID m_flight_loop_id {nullptr};

    std::unique_ptr<text_logger> m_plugin_log;
    std::unique_ptr<midi_logger> m_midi_log;

    std::map<std::string, std::shared_ptr<info_msg>> m_info_msg;

    std::unique_ptr<env_xplane> m_env;
    std::unique_ptr<settings> m_settings;
    std::unique_ptr<profile> m_profile;

    std::unique_ptr<menu> m_menu;
    std::unique_ptr<inbound_worker> m_worker;

    // references for custom datarefs
    XPLMDataRef m_drf_sublayer {nullptr};

    // references for custom commands
    XPLMCommandRef m_cmd_show_messages {nullptr};
    XPLMCommandRef m_cmd_show_profile {nullptr};
    XPLMCommandRef m_cmd_reload_profile {nullptr};
    XPLMCommandRef m_cmd_toggle_sublayer {nullptr};

    // internal sublayer number
    int m_sublayer {0};

    // list of all plugin windows
    std::map<window_type, std::shared_ptr<xplane_window>> m_windows;
};

} // Namespace xmidictrl

#endif // PLUGIN_H
