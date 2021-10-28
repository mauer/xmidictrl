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

// Standard
#include <string>
#include <utility>

#include <XPLMUtilities.h>

// XMidiCtrl
#include "AboutDialog.h"
#include "Config.h"
#include "Logger.h"
#include "MessageWindow.h"
#include "MidiDevicesDialog.h"
#include "Plugin.h"
#include "SettingsDialog.h"
#include "Version.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Plugin::Plugin() {
    m_flightLoopId = nullptr;
    m_profile      = nullptr;
    m_devices      = nullptr;

    // create the integration to X-Plane
    m_xplane = std::make_shared<XPlane>();

    XPLMDebugString("Plugin XMidiCtrl");
    XPLMDebugString(XMIDICTRL_NAME);
    XPLMDebugString(m_xplane->xplanePath().data());

    Logger::Instance().initialise(m_xplane->xplanePath(), XMIDICTRL_NAME);

    // create the event handler, which process all midi events
    m_eventHandler = std::make_shared<EventHandler>();

    // initialize the logger
    LOG_INFO << "Plugin " << XMIDICTRL_FULL_NAME << " loaded successfully" << LOG_END
}


/**
 * Destructor
 */
Plugin::~Plugin() {
    m_flightLoopId = nullptr;
    m_windows.clear();

    // close all midi connections
    closeMidiConnections();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and return the plugin instance
 */
Plugin& Plugin::Instance() {
    static Plugin plugin;
    return plugin;
}


/**
 * Callback for the flight loop
 */
float Plugin::callbackFlightLoop(float elapsedMe, float elapsedSim, int counter, void *refcon) {
    // call instance method
    if (refcon != nullptr) {
        auto midiCtrl = static_cast<Plugin*>(refcon);
        midiCtrl->processFlightLoop(elapsedMe, elapsedSim, counter);
    }

    return FLIGHTLOOP_INTERVAL;
}


/**
 * Process the flight loop
 */
void Plugin::processFlightLoop(float elapsedMe, float elapsedSim, int counter) {
    // process midi events
    m_eventHandler->processEvents();

    // display screen messages
    showMessages();
}


/**
 * Enable the plugin
 */
void Plugin::enablePlugin() {
    LOG_INFO << "Plugin " << XMIDICTRL_FULL_NAME << " enabled" << LOG_END

    // create all required menu entries
    m_menu.createMenu();

    // initialise configured midi devices
    initialiseDevices();

    // register flight loop
    XPLMCreateFlightLoop_t params = {sizeof(XPLMCreateFlightLoop_t),
                                     xplm_FlightLoop_Phase_BeforeFlightModel,
                                     callbackFlightLoop, this};
    m_flightLoopId = XPLMCreateFlightLoop(&params);

    if (m_flightLoopId != nullptr) {
        LOG_DEBUG << "Flight loop successfully registered" << LOG_END

        XPLMScheduleFlightLoop(m_flightLoopId, FLIGHTLOOP_INTERVAL, true);
    } else
        LOG_ERROR << "Could not create flight loop" << LOG_END
}


/**
 * Disable the plugin
 */
void Plugin::disablePlugin() {
    // delete the menu entries
    m_menu.deleteMenu();

    // destroy the flight loop
    if (m_flightLoopId != nullptr) {
        XPLMDestroyFlightLoop(m_flightLoopId);
        m_flightLoopId = nullptr;

        LOG_DEBUG << "Flight loop destroyed" << LOG_END
    }

    closeMidiConnections();

    LOG_INFO << "Plugin disabled" << LOG_END
}


/**
 * Load the profile for the current aircraft
 */
void Plugin::loadAircraftProfile() {
    LOG_DEBUG << "Start loading aircraft profile" << LOG_END

    // reload settings for current aircraft
    if (m_profile == nullptr)
        m_profile = std::make_shared<Profile>(m_xplane);

    m_profile->load();
    initialiseDevices();
}


/**
 * Unload the aircraft profile
 */
void Plugin::unloadAircraftProfile() {
    LOG_DEBUG << "Start to unload the current aircraft profile" << LOG_END

    // clear current aircraft settings
    if (m_profile != nullptr)
        m_profile->clear();

    closeMidiConnections();
}


/**
 * Add a mapped event to the queue
 */
void Plugin::addMappedEvent(const MappedEvent::ptr& mappedEvent) {
    m_eventHandler->addMappedEvent(mappedEvent);
}


/**
 * Search for available midi devices
 */
void Plugin::showMidiDevicesDialog() {
    createWindow(WindowType::MidiDevicesDialog);
}


/**
 * Show general settings dialog
 */
void Plugin::showSettingsDialog() {
    createWindow(WindowType::SettingsDialog);
}

/**
 * Show the about dialog
 */
void Plugin::showAboutDialog() {
    createWindow(WindowType::AboutDialog);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Initialise configured midi devices
 */
void Plugin::initialiseDevices() {
    LOG_INFO << "Initialise MIDI devices" << LOG_END

    closeMidiConnections();

    if (m_profile != nullptr)
        m_devices = m_profile->createMidiDevices();

    if (m_devices != nullptr && m_devices->size() > 0)
        m_devices->openConnections();
}


/**
 * Close all open midi connections
 */
void Plugin::closeMidiConnections() {
    LOG_DEBUG << "Close all midi connections" << LOG_END

    if (m_devices != nullptr) {
        m_devices->closeConnections();
        m_devices->clear();
    }
}


/**
 * Display messages in the message queue
 */
void Plugin::showMessages() {
    if (Logger::Instance().messages()->size() > 0)
        showMessageWindow();
}


/**
 * Show the global message window
 */
void Plugin::showMessageWindow() {

}


/**
 * Create and returns windows
 */
void Plugin::createWindow(WindowType windowType) {
    // check if the window is already created
    try {
        m_windows.at(windowType);
    } catch (std::out_of_range&) {
    }

    // looks like we have to create it
    XPlaneWindow::ptr window;
    switch (windowType) {
        case WindowType::AboutDialog:
            window = std::make_shared<AboutDialog>();
            break;

        case WindowType::MessageWindow:
            window = std::make_shared<MessageWindow>();
            break;

        case WindowType::MidiDevicesDialog:
            window = std::make_shared<MidiDevicesDialog>();
            break;

        case WindowType::SettingsDialog:
            window = std::make_shared<SettingsDialog>();
            break;
    }

    if (window)
        m_windows.emplace(windowType, window);
}

} // Namespace XMidiCtrl