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

#include "plugin.h"

// XMidiCtrl
#include "about_window.h"
#include "conversions.h"
#include "devices_window.h"
#include "info_window.h"
#include "messages_window.h"
#include "profile_window.h"
#include "settings_window.h"
#include "version.h"
#include "xmc_utils.h"

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

    // create the integration to X-Plane
    m_xp = std::make_unique<xplane>(*m_plugin_log);

    // load general settings
    m_settings = std::make_unique<settings>(*m_plugin_log, *m_xp);

    // initialize our logging system
    if (utils::create_preference_folders(*m_plugin_log, *m_xp)) {
        m_plugin_log->enable_file_logging(m_xp->preferences_path());
        m_plugin_log->set_debug_mode(m_settings->debug_mode());
        m_plugin_log->set_max_size(m_settings->max_text_messages());
    } else {
        m_plugin_log->info("Cannot create preference folder for " XMIDICTRL_FULL_NAME);
        XPLMDebugString(std::string_view("Cannot create preference folder for " XMIDICTRL_FULL_NAME).data());
    }

    m_plugin_log->info("Plugin " XMIDICTRL_FULL_NAME " loaded successfully");
    XPLMDebugString(std::string_view("Plugin " XMIDICTRL_FULL_NAME " loaded successfully").data());

    // create the menu
    m_menu = std::make_unique<menu>();

    // create the midi log
    m_midi_log = std::make_unique<midi_logger>(*m_settings);

    // create the aircraft profile
    m_profile = std::make_unique<profile>(*m_plugin_log, *m_midi_log, *m_xp, *m_settings);

    // create the inbound worker
    m_worker = std::make_unique<inbound_worker>();
}


/**
 * Destructor
 */
plugin::~plugin()
{
    m_profile.reset();
    m_menu.reset();
    m_worker.reset();

    m_flight_loop_id = nullptr;
    m_windows.clear();

    m_midi_log.reset();

    m_plugin_log->info("Plugin " XMIDICTRL_FULL_NAME " unloaded successfully");
    m_plugin_log.reset();

    m_xp.reset();

    XPLMDebugString(std::string_view("Plugin " XMIDICTRL_FULL_NAME " unloaded successfully").data());
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and return the plugin instance
 */
plugin &plugin::instance()
{
    static plugin instance;
    return instance;
}


/**
 * Callback for the flight loop
 */
float plugin::callback_flight_loop(float in_elapsed_me, float in_elapsed_sim, int in_counter, void *in_refcon)
{
    // call instance method
    if (in_refcon != nullptr) {
        auto instance = static_cast<plugin *>(in_refcon);
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
    std::string sl_value {};

    // are sublayers active?
    if (!m_profile->sl_dataref().empty())
        m_xp->datarefs().read(*m_plugin_log, m_profile->sl_dataref(), sl_value);

    // process inbound tasks
    m_worker->process(sl_value);

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

    if (m_info_msg.empty())
        return;

    // remove expired messages
    for (auto it = m_info_msg.cbegin(); it != m_info_msg.cend();) {
        auto msg = it->second;

        if (msg->exp_time < std::chrono::system_clock::now())
            it = m_info_msg.erase(it);
        else
            ++it;
    }

    // show or hide dialog
    auto win = create_window(window_type::info_window);

    if (win != nullptr) {
        if (m_info_msg.empty())
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
    conversions::create_preference_folders(*m_plugin_log, *m_xp);
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
        if (m_settings->show_messages())
            show_messages_window();
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
 * Display an info message on the screen
 */
void plugin::show_info_message(std::string_view in_id, std::string_view in_msg, int in_seconds)
{
    if (m_settings->info_disabled())
        return;

    std::shared_ptr<info_msg> msg;

    if (in_seconds == -1)
        msg = std::make_shared<info_msg>(m_settings->info_seconds());
    else
        msg = std::make_shared<info_msg>(in_seconds);

    msg->id = in_id;
    msg->text = in_msg;

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    m_info_msg.insert_or_assign(msg->id, msg);
}


/**
 * Add an inbound task to the worker
 */
void plugin::add_inbound_task(const std::shared_ptr<inbound_task> &in_task)
{
    if (m_worker != nullptr)
        m_worker->add_task(in_task);
}


/**
 * Show the log messages dialog
 */
void plugin::show_messages_window()
{
    auto window = create_window(window_type::messages_window);
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

    m_xp->datarefs().write(*m_plugin_log, "xmidictrl/sublayer", "0");
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
    m_cmd_show_messages = XPLMCreateCommand("xmidictrl/show_messages_window", "Show the messages window");
    XPLMRegisterCommandHandler(m_cmd_show_messages,
                               command_handler,
                               1,
                               (void *) COMMAND_MESSAGE_WINDOW);

    m_cmd_show_profile = XPLMCreateCommand("xmidictrl/show_profile_window", "Show the aircraft profile window");
    XPLMRegisterCommandHandler(m_cmd_show_profile,
                               command_handler,
                               1,
                               (void *) COMMAND_PROFILE_WINDOW);

    m_cmd_reload_profile = XPLMCreateCommand("xmidictrl/reload_profile", "Reload the aircraft profile");
    XPLMRegisterCommandHandler(m_cmd_reload_profile,
                               command_handler,
                               1,
                               (void *) COMMAND_RELOAD_PROFILE);

    m_cmd_toggle_sublayer = XPLMCreateCommand("xmidictrl/toggle_sublayer",
                                              "Toggle dataref 'xmidictrl/sublayer' between 0 and 1");
    XPLMRegisterCommandHandler(m_cmd_toggle_sublayer,
                               command_handler,
                               1,
                               (void *) COMMAND_TOGGLE_SUBLAYER);
}


/**
 * Remove all custom commands
 */
void plugin::remove_commands()
{
    XPLMUnregisterCommandHandler(m_cmd_show_messages, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_show_profile, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_reload_profile, command_handler, 0, nullptr);
    XPLMUnregisterCommandHandler(m_cmd_toggle_sublayer, command_handler, 0, nullptr);
}


/**
 * Read handler for dataref xmidictrl/sublayer
 */
int plugin::read_drf_sublayer(void *in_refcon)
{
    if (in_refcon != nullptr) {
        auto instance = static_cast<plugin *>(in_refcon);
        return instance->sublayer();
    }

    return 0;
}


/**
 * Write handler for dataref xmidictrl/sublayer
 */
void plugin::write_drf_sublayer(void *in_refcon, int in_value)
{
    if (in_refcon != nullptr) {
        auto instance = static_cast<plugin *>(in_refcon);
        instance->set_sublayer(in_value);
    }
}


/**
 * Handler for custom commands
 */
int plugin::command_handler([[maybe_unused]] XPLMCommandRef in_command, XPLMCommandPhase in_phase, void *in_refcon)
{
    if (in_phase != xplm_CommandEnd)
        return 1;

    plugin::instance().show_info_message("CMD", "Command executed");

    if (!strcmp((const char *) in_refcon, COMMAND_MESSAGE_WINDOW))
        plugin::instance().show_messages_window();
    else if (!strcmp((const char *) in_refcon, COMMAND_PROFILE_WINDOW))
        plugin::instance().show_profile_window();
    else if (!strcmp((const char *) in_refcon, COMMAND_RELOAD_PROFILE))
        plugin::instance().load_profile();
    else if (!strcmp((const char *) in_refcon, COMMAND_TOGGLE_SUBLAYER))
        plugin::instance().toggle_sublayer();

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
    } catch (std::out_of_range &) {
        window = nullptr;
    }

    // looks like we have to create it
    switch (in_type) {
        case window_type::about_window:
            window = std::make_shared<about_window>(*m_plugin_log, *m_xp);
            break;

        case window_type::messages_window:
            window = std::make_shared<messages_window>(*m_plugin_log, *m_midi_log, *m_xp, *m_settings);
            break;

        case window_type::devices_window:
            window = std::make_shared<devices_window>(*m_plugin_log, *m_xp);
            break;

        case window_type::info_window:
            window = std::make_shared<info_window>(*m_plugin_log, *m_xp, *m_settings, m_info_msg);
            break;

        case window_type::profile_window:
            window = std::make_shared<profile_window>(*m_plugin_log, *m_xp, *m_profile);
            break;

        case window_type::settings_window:
            window = std::make_shared<settings_window>(*m_plugin_log, *m_xp, *m_settings);
            break;
    }

    if (window != nullptr)
        m_windows.emplace(in_type, window);

    return window;
}

} // Namespace xmidictrl
