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
#include "config.h"
#include "devices_window.h"
#include "logger.h"
#include "messages_window.h"
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
    // create the integration to X-Plane
    m_xp = std::make_shared<xplane>();

    // load general settings
    m_settings = std::make_shared<settings>(m_xp);

    // initialize our logging instance
    logger::instance().init(m_xp->xplane_path(), m_settings);
    LOG_ALL << "Plugin " << XMIDICTRL_FULL_NAME << " loaded successfully" << LOG_END

    // create the menu
    m_menu = std::make_unique<menu>();

    // create the aircraft profile
    m_profile = std::make_shared<profile>(m_xp);
}


/**
 * Destructor
 */
plugin::~plugin()
{
    m_flight_loop_id = nullptr;
    m_windows.clear();

    LOG_ALL << "Plugin " << XMIDICTRL_FULL_NAME << " unloaded successfully" << LOG_END
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
float plugin::callback_flight_loop(float elapsed_me, float elapsed_sim, int counter, void *refcon)
{
    // call instance method
    if (refcon != nullptr) {
        auto midiCtrl = static_cast<plugin *>(refcon);
        midiCtrl->process_flight_loop(elapsed_me, elapsed_sim, counter);
    }

    return FLIGHTLOOP_INTERVAL;
}


/**
 * Process the flight loop
 */
void plugin::process_flight_loop(float elapsed_me, float elapsed_sim, int counter)
{
    m_profile->process();
}


/**
 * Enable the plugin
 */
void plugin::enable()
{
    LOG_INFO << "Plugin enabled" << LOG_END

    // create all required menu entries
    m_menu->create_menu();

    // register flight loop
    XPLMCreateFlightLoop_t params = {sizeof(XPLMCreateFlightLoop_t), xplm_FlightLoop_Phase_BeforeFlightModel,
                                     callback_flight_loop, this};
    m_flight_loop_id = XPLMCreateFlightLoop(&params);

    if (m_flight_loop_id != nullptr) {
        LOG_DEBUG << "Flight loop registered" << LOG_END

        XPLMScheduleFlightLoop(m_flight_loop_id, FLIGHTLOOP_INTERVAL, true);
    } else {
        LOG_ERROR << "Could not register flight loop" << LOG_END
    }

    // check if our directory already exists in the preference folder
    utils::create_preference_folders(m_xp);
}


/**
 * Disable the plugin
 */
void plugin::disable()
{
    // delete the menu entries
    m_menu->remove_menu();

    // destroy the flight loop
    if (m_flight_loop_id != nullptr) {
        XPLMDestroyFlightLoop(m_flight_loop_id);
        m_flight_loop_id = nullptr;

        LOG_DEBUG << "Flight loop destroyed" << LOG_END
    }

    // close current profile
    close_profile();

    LOG_INFO << "Plugin disabled" << LOG_END
}


/**
 * Load the profile for the current aircraft
 */
void plugin::load_profile()
{
    if (!m_profile->load() && m_profile->has_errors()) {
        if (m_settings->get_show_messages())
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
 * Show the log messages dialog
 */
void plugin::show_messages_window()
{
    create_window(window_type::messages_window);
}


/**
 * Search for available midi devices
 */
void plugin::show_devices_window()
{
    create_window(window_type::devices_window);
}


/**
 * Show the aircraft profile window
 */
void plugin::show_profile_window()
{
    create_window(window_type::profile_window);
}


/**
 * Show general settings dialog
 */
void plugin::show_settings_window()
{
    create_window(window_type::settings_window);
}


/**
 * Show the about dialog
 */
void plugin::show_about_window()
{
    create_window(window_type::about_window);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and returns windows
 */
void plugin::create_window(window_type windowType)
{
    std::shared_ptr<xplane_window> window;

    // check if the window is already created
    try {
        window = m_windows.at(windowType);
        window->show();
        return;
    } catch (std::out_of_range &) {
        window = nullptr;
    }

    // looks like we have to create it
    switch (windowType) {
        case window_type::about_window:
            window = std::make_shared<about_window>(m_xp);
            break;

        case window_type::messages_window:
            window = std::make_shared<messages_window>(m_xp);
            break;

        case window_type::devices_window:
            window = std::make_shared<devices_window>(m_xp);
            break;

        case window_type::profile_window:
            window = std::make_shared<profile_window>(m_xp, m_profile);
            break;

        case window_type::settings_window:
            window = std::make_shared<settings_window>(m_xp, m_settings);
            break;
    }

    if (window)
        m_windows.emplace(windowType, window);
}

} // Mamespace xmidictrl