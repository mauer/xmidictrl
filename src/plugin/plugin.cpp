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

#include "plugin.h"

// XMidiCtrl
#include "about_window.h"
#include "config.h"
#include "logger.h"
#include "messages_window.h"
#include "devices_window.h"
#include "settings_window.h"
#include "Version.h"

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
    m_xplane = std::make_shared<xplane>();

    // initialize our logging instance
    logger::instance().init(m_xplane->xplane_path());
    LOG_ALL << "Plugin " << XMIDICTRL_FULL_NAME << " loaded successfully" << LOG_END

    // load general settings
    m_settings = std::make_shared<settings>(m_xplane);

    // set general settings
    logger::instance().set_log_level(m_settings->loglevel());

    // create the menu
    m_menu = std::make_unique<menu>();

    // create the aircraft profile
    m_profile = std::make_unique<profile>(m_xplane);
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
    XPLMCreateFlightLoop_t params = {sizeof(XPLMCreateFlightLoop_t),
                                     xplm_FlightLoop_Phase_BeforeFlightModel,
                                     callback_flight_loop, this};
    m_flight_loop_id = XPLMCreateFlightLoop(&params);

    if (m_flight_loop_id != nullptr) {
        LOG_DEBUG << "Flight loop registered" << LOG_END

        XPLMScheduleFlightLoop(m_flight_loop_id, FLIGHTLOOP_INTERVAL, true);
    } else
        LOG_ERROR << "Could not register flight loop" << LOG_END
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
    if (!m_profile->load() || m_profile->has_errors()) {
        if (m_settings->show_messages())
            show_messages_dialog();
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
void plugin::show_messages_dialog()
{
    create_window(WindowType::MessagesDialog);
}


/**
 * Search for available midi devices
 */
void plugin::show_devices_dialog()
{
    create_window(WindowType::MidiDevicesDialog);
}


/**
 * Show general settings dialog
 */
void plugin::show_settings_dialog()
{
    create_window(WindowType::SettingsDialog);
}


/**
 * Show the about dialog
 */
void plugin::show_about_dialog()
{
    create_window(WindowType::AboutDialog);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and returns windows
 */
void plugin::create_window(WindowType windowType)
{
    XPlaneWindow::ptr window;

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
        case WindowType::AboutDialog:
            window = std::make_shared<about_window>();
            break;

        case WindowType::MessagesDialog:
            window = std::make_shared<messages_window>();
            break;

        case WindowType::MidiDevicesDialog:
            window = std::make_shared<devices_window>();
            break;

        case WindowType::SettingsDialog:
            window = std::make_shared<settings_window>(m_settings);
            break;
    }

    if (window)
        m_windows.emplace(windowType, window);
}

} // Mamespace XMidiCtrl