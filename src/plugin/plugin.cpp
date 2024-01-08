//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

#include "plugin.h"

// XMidiCtrl
#include "about_window.h"
#include "conversions.h"
#include "devices_window.h"
#include "info_window.h"
#include "log_viewer.h"
#include "midi_watcher.h"
#include "profile_window.h"
#include "settings_window.h"
#include "version.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
plugin::plugin()
{
    // create logger
    m_plugin_log = std::make_unique<text_logger>();

    // create the environment for X-Plane
    m_env = std::make_unique<env_xplane>(*m_plugin_log);

    // initialize our logging system
    if (m_env->create_preference_folders(*m_plugin_log)) {
        m_plugin_log->enable_file_logging(m_env->preferences_path());
        m_plugin_log->set_debug_mode(m_env->settings().debug_mode());
        m_plugin_log->set_max_size(m_env->settings().max_text_messages());
    } else {
        m_plugin_log->info("Cannot create preference folder for " XMIDICTRL_FULL_NAME);
        XPLMDebugString(std::string_view("Cannot create preference folder for " XMIDICTRL_FULL_NAME).data());
    }

    m_plugin_log->info("Plugin " XMIDICTRL_FULL_NAME " loaded successfully");
    XPLMDebugString(std::string_view("Plugin " XMIDICTRL_FULL_NAME " loaded successfully").data());

    // create the menu
    m_menu = std::make_unique<menu>();

    // create the midi log
    m_midi_log = std::make_unique<midi_logger>(m_env->settings().log_midi(), m_env->settings().max_midi_messages());

    // create the aircraft profile
    m_profile = std::make_unique<profile>(*m_plugin_log, *m_midi_log, *m_env);
}


/**
 * Destructor
 */
plugin::~plugin()
{
    m_profile.reset();
    m_menu.reset();

    m_flight_loop_id = nullptr;
    m_windows.clear();

    m_midi_log.reset();

    m_plugin_log->info("Plugin " XMIDICTRL_FULL_NAME " unloaded successfully");
    m_plugin_log.reset();

    m_env.reset();

    XPLMDebugString(std::string_view("Plugin " XMIDICTRL_FULL_NAME " unloaded successfully").data());
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and return the plugin instance
 */
plugin& plugin::instance()
{
    static plugin instance;
    return instance;
}


/**
 * Callback for the flight loop
 */
float plugin::callback_flight_loop(float in_elapsed_me, float in_elapsed_sim, int in_counter, void* in_refcon)
{
    // call instance method
    if (in_refcon != nullptr) {
        auto instance = static_cast<plugin*>(in_refcon);
        instance->process_flight_loop(in_elapsed_me, in_elapsed_sim, in_counter);
    }

    return FLIGHTLOOP_INTERVAL;
}


/**
 * Process the flight loop
 */
void plugin::process_flight_loop([[maybe_unused]] float in_elapsed_me,
                                 [[maybe_unused]] float in_elapsed_sim,
                                 [[maybe_unused]] int in_counter)
{
    // update all sublayer datarefs
    m_profile->devices().update_sl_values(*m_plugin_log, *m_env);

    // process inbound tasks
    m_env->worker().process();

    // process outbound tasks
    m_profile->process(*m_plugin_log);

    // process info messages
    process_info_messages();
}


/**
 * Process into messages
 */
void plugin::process_info_messages()
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    if (m_env->info_messages().empty())
        return;

    // remove expired messages
    for (auto it = m_env->info_messages().cbegin(); it != m_env->info_messages().cend();) {
        auto msg = it->second;

        if (msg->exp_time < std::chrono::system_clock::now())
            it = m_env->info_messages().erase(it);
        else
            ++it;
    }

    // show or hide dialog
    auto win = create_window(window_type::info_window);

    if (win != nullptr) {
        if (m_env->info_messages().empty())
            win->hide();
        else
            win->show();
    }
}


/**
 * Enable the plugin
 */
void plugin::enable()
{
    m_plugin_log->info("Plugin enabled");

    // create all required menu entries
    m_menu->create_menu();

    // create the custom datarefs and commands
    create_datarefs();
    create_commands();

    // register flight loop
    XPLMCreateFlightLoop_t params = {sizeof(XPLMCreateFlightLoop_t), xplm_FlightLoop_Phase_BeforeFlightModel,
                                     callback_flight_loop, this};
    m_flight_loop_id = XPLMCreateFlightLoop(&params);

    if (m_flight_loop_id != nullptr) {
        m_plugin_log->debug("Flight loop registered");

        XPLMScheduleFlightLoop(m_flight_loop_id, FLIGHTLOOP_INTERVAL, true);
    } else {
        m_plugin_log->error("Could not register flight loop");
    }

    // check if our directory already exists in the preference folder
    m_env->create_preference_folders(*m_plugin_log);
}


/**
 * Disable the plugin
 */
void plugin::disable()
{
    // remove the menu entries
    m_menu->remove_menu();

    // remove the custom datarefs and commands
    remove_commands();
    remove_datarefs();

    // destroy the flight loop
    if (m_flight_loop_id != nullptr) {
        XPLMDestroyFlightLoop(m_flight_loop_id);
        m_flight_loop_id = nullptr;

        m_plugin_log->debug("Flight loop destroyed");
    }

    // close current profile
    close_profile();

    m_plugin_log->info("Plugin disabled");
}


/**
 * Load the profile for the current aircraft
 */
void plugin::load_profile()
{
    if (!m_profile->load() || m_profile->has_errors()) {
        if (m_env->settings().show_errors())
            show_log_viewer();
    }
}


/**
 * Unload the aircraft profile
 */
void plugin::close_profile()
{
    m_profile->close();
}


/**
 *
 */
void plugin::add_virtual_midi_message(unsigned char in_cc, unsigned char in_value)
{
    if (m_profile->loaded()) {
        auto virtual_dev = m_profile->devices().find_virtual_device();

        if (virtual_dev != nullptr) {
            m_plugin_log->debug("Virtual device found");

            virtual_dev->process_inbound_message(static_cast<unsigned char>(m_env->settings().virtual_channel()),
                                                 in_cc,
                                                 in_value);
        } else {
            m_plugin_log->debug("No Virtual device found");
        }
    }
}


/**
 * Show the log viewer
 */
void plugin::show_log_viewer()
{
    auto window = create_window(window_type::log_viewer);
    window->show();
}


/**
 * Show the MIDI watcher
 */
void plugin::show_midi_watcher()
{
    auto window = create_window(window_type::midi_watcher);
    window->show();
}


/**
 * Search for available midi devices
 */
void plugin::show_devices_window()
{
    auto window = create_window(window_type::devices_window);
    window->show();
}


/**
 * Show the aircraft profile window
 */
void plugin::show_profile_window()
{
    auto window = create_window(window_type::profile_window);
    window->show();
}


/**
 * Show general settings dialog
 */
void plugin::show_settings_window()
{
    auto window = create_window(window_type::settings_window);
    window->show();
}


/**
 * Show the about dialog
 */
void plugin::show_about_window()
{
    auto window = create_window(window_type::about_window);
    window->show();
}


/**
 * Return the current sublayer
 */
int plugin::sublayer() const
{
    return m_sublayer;
}


/**
 * Set the current sublayer
 */
void plugin::set_sublayer(int in_value)
{
    m_sublayer = in_value;
}


/**
 * Toggle the sublayer between 0 and 1
 */
void plugin::toggle_sublayer()
{
    if (m_sublayer == 1)
        m_sublayer = 0;
    else
        m_sublayer = 1;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create custom datarefs
 */
void plugin::create_datarefs()
{
    m_drf_sublayer = XPLMRegisterDataAccessor("xmidictrl/sublayer",
                                              xplmType_Int,
                                              1,
                                              read_drf_sublayer, write_drf_sublayer,
                                              nullptr, nullptr,
                                              nullptr, nullptr,
                                              nullptr, nullptr,
                                              nullptr, nullptr,
                                              nullptr, nullptr,
                                              this, this);

    m_env->drf().write(*m_plugin_log, "xmidictrl/sublayer", "0");
}


/**
 * Remove custom datarefs
 */
void plugin::remove_datarefs()
{
    XPLMUnregisterDataAccessor(m_drf_sublayer);
}


/**
 * Create customs commands
 */
void plugin::create_commands()
{
    // general plugin commands
    m_cmd_log_viewer = XPLMCreateCommand("xmidictrl/show_log_viewer", "Show the log viewer");
    XPLMRegisterCommandHandler(m_cmd_log_viewer,
                               command_handler,
                               1,
                               (void*) COMMAND_LOG_VIEWER);

    m_cmd_midi_watcher = XPLMCreateCommand("xmidictrl/show_midi_watcher", "Show the MIDI watcher");
    XPLMRegisterCommandHandler(m_cmd_midi_watcher,
                               command_handler,
                               1,
                               (void*) COMMAND_MIDI_WATCHER);

    m_cmd_show_profile = XPLMCreateCommand("xmidictrl/show_profile_window", "Show the aircraft profile window");
    XPLMRegisterCommandHandler(m_cmd_show_profile,
                               command_handler,
                               1,
                               (void*) COMMAND_PROFILE_WINDOW);

    m_cmd_reload_profile = XPLMCreateCommand("xmidictrl/reload_profile", "Reload the aircraft profile");
    XPLMRegisterCommandHandler(m_cmd_reload_profile,
                               command_handler,
                               1,
                               (void*) COMMAND_RELOAD_PROFILE);

    m_cmd_toggle_sublayer = XPLMCreateCommand("xmidictrl/toggle_sublayer",
                                              "Toggle dataref 'xmidictrl/sublayer' between 0 and 1");
    XPLMRegisterCommandHandler(m_cmd_toggle_sublayer,
                               command_handler,
                               1,
                               (void*) COMMAND_TOGGLE_SUBLAYER);

    // virtual midi commands
    for (int i = 0; i <= 127; i++) {
        std::string cmd_name = "xmidictrl/midi/send_midi_cc_msg_" + conversions::int_to_string(i, 3);

        std::string cmd_descr = "Send virtual MIDI message (CH "
                                + std::to_string(m_env->settings().virtual_channel()) + " CC "
                                + std::to_string(i) + ")";

        m_cmd_vmidi_message[i] = XPLMCreateCommand(cmd_name.data(), cmd_descr.data());

        int* ptr = (int*) malloc(sizeof(int));
        *ptr = i;

        XPLMRegisterCommandHandler(m_cmd_vmidi_message[i],
                                   virtual_midi_command_handler,
                                   1,
                                   ptr);
    }
}


/**
 * Remove all custom commands
 */
void plugin::remove_commands()
{
    XPLMUnregisterCommandHandler(m_cmd_log_viewer, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_midi_watcher, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_show_profile, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_reload_profile, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_toggle_sublayer, command_handler, 0, nullptr);

    for (int i = 0; i <= 127; i++)
        XPLMUnregisterCommandHandler(m_cmd_vmidi_message[i], nullptr, 0, nullptr);
}


/**
 * Read handler for dataref xmidictrl/sublayer
 */
int plugin::read_drf_sublayer(void* in_refcon)
{
    if (in_refcon != nullptr) {
        auto instance = static_cast<plugin*>(in_refcon);
        return instance->sublayer();
    }

    return 0;
}


/**
 * Write handler for dataref xmidictrl/sublayer
 */
void plugin::write_drf_sublayer(void* in_refcon, int in_value)
{
    if (in_refcon != nullptr) {
        auto instance = static_cast<plugin*>(in_refcon);
        instance->set_sublayer(in_value);
    }
}


/**
 * Handler for custom commands
 */
int plugin::command_handler([[maybe_unused]] XPLMCommandRef in_command, XPLMCommandPhase in_phase, void* in_refcon)
{
    if (in_phase != xplm_CommandEnd)
        return 1;

    if (!strcmp((const char*) in_refcon, COMMAND_LOG_VIEWER))
        plugin::instance().show_log_viewer();
    else if (!strcmp((const char*) in_refcon, COMMAND_MIDI_WATCHER))
        plugin::instance().show_midi_watcher();
    else if (!strcmp((const char*) in_refcon, COMMAND_PROFILE_WINDOW))
        plugin::instance().show_profile_window();
    else if (!strcmp((const char*) in_refcon, COMMAND_RELOAD_PROFILE))
        plugin::instance().load_profile();
    else if (!strcmp((const char*) in_refcon, COMMAND_TOGGLE_SUBLAYER))
        plugin::instance().toggle_sublayer();

    // disable further processing by X-Plane.
    return 0;
}


/**
 * Handler for virtual midi commands
 */
int plugin::virtual_midi_command_handler([[maybe_unused]] XPLMCommandRef in_command,
                                         XPLMCommandPhase in_phase,
                                         void* in_refcon)
{
    if (in_phase == xplm_CommandContinue)
        return 1;

    // get the cc number
    int cc = *((int*) in_refcon);

    switch (in_phase) {
        case xplm_CommandBegin:
            plugin::instance().add_virtual_midi_message(static_cast<unsigned char>(cc), MIDI_DATA_2_MAX);
            break;

        case xplm_CommandEnd:
            plugin::instance().add_virtual_midi_message(static_cast<unsigned char>(cc), MIDI_DATA_2_MIN);
            break;

        default:
            break;
    }

    // disable further processing by X-Plane.
    return 0;
}


/**
 * Create and returns windows
 */
std::shared_ptr<xplane_window> plugin::create_window(window_type in_type)
{
    std::shared_ptr<xplane_window> window;

    // check if the window is already created
    try {
        window = m_windows.at(in_type);
        window->show();
        return window;
    } catch (std::out_of_range&) {
        window = nullptr;
    }

    // looks like we have to create it
    switch (in_type) {
        case window_type::about_window:
            window = std::make_shared<about_window>(*m_plugin_log, *m_env);
            break;

        case window_type::log_viewer:
            window = std::make_shared<log_viewer>(*m_plugin_log, *m_env);
            break;

        case window_type::midi_watcher:
            window = std::make_shared<midi_watcher>(*m_plugin_log, *m_midi_log, *m_env);
            break;

        case window_type::devices_window:
            window = std::make_shared<devices_window>(*m_plugin_log, *m_env);
            break;

        case window_type::info_window:
            window = std::make_shared<info_window>(*m_plugin_log, *m_env);
            break;

        case window_type::profile_window:
            window = std::make_shared<profile_window>(*m_plugin_log, *m_env, *m_profile);
            break;

        case window_type::settings_window:
            window = std::make_shared<settings_window>(*m_plugin_log, *m_env);
            break;
    }

    if (window != nullptr)
        m_windows.emplace(in_type, window);

    return window;
}

} // Namespace xmidictrl