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

// X-Plane SDK
#include "XPLMPlugin.h"

// XMidiCtrl
#include "Config.h"
#include "Logger.h"
#include "Plugin.h"
#include "ui/AboutDialog.h"
#include "ui/MessageWindow.h"
#include "ui/MidiDevicesDialog.h"
#include "ui/SettingsDialog.h"
#include "Version.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Plugin::Plugin() {
    m_pluginId     = XPLMGetMyID();
    m_flightLoopId = nullptr;

    // create the environment, which holds all data
    m_environment = std::make_shared<Environment>();

    // read the general settings first
    m_settings = std::make_shared<Settings>();

    // create the event handler, which process all midi events
    m_eventHandler = std::make_shared<EventHandler>(m_environment);

    // create device list
    m_devices = std::make_shared<DeviceList>();

    // create a list for all message to be displayed on the screen
    m_messages = std::make_shared<MessageList>(m_settings);

    LOG_INFO << "Plugin " << XMIDICTRL_NAME << " " << XMIDICTRL_VERSION_STR << " loaded successfully" << LOG_END
}


/**
 * Destructor
 */
Plugin::~Plugin() {
    m_flightLoopId = nullptr;
    m_windows.clear();

    // close and destroy all midi connections and devices
    closeMidiConnections();
    destroyDeviceList();
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
        auto midiCtrl = static_cast< Plugin * >(refcon);
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

    // display messages
    showMessages();
}


/**
 * Enable the plugin
 */
void Plugin::enablePlugin() {
    LOG_INFO << "PLUGIN :: Enable plugin" << LOG_END

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
        LOG_INFO << "PLUGIN :: Flight loop successfully registered" << LOG_END

        XPLMScheduleFlightLoop(m_flightLoopId, FLIGHTLOOP_INTERVAL, true);
    } else
        LOG_ERROR << "PLUGIN :: Could not create flight loop" << LOG_END
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

        LOG_INFO << "PLUGIN :: Flight loop destroyed" << LOG_END
    }

    closeMidiConnections();
    destroyDeviceList();

    LOG_INFO << "PLUGIN :: Disabled plugin" << LOG_END
}


/**
 * Load the profile for the current aircraft
 */
void Plugin::loadAircraftProfile() {
    LOG_INFO << "PLUGIN :: Load aircraft profile" << LOG_END

    // reload settings for current aircraft
    m_profile.load();

    initialiseDevices();
}


/**
 * Clear aircraft profile
 */
void Plugin::clearAircraftProfile() {
    LOG_INFO << "PLUGIN :: Clear aircraft profile" << LOG_END

    // clear current aircraft settings
    m_profile.clear();

    // close active midi connections
    closeMidiConnections();
    destroyDeviceList();
}


/**
 * Add a mapped event to the queue
 */
void Plugin::addMappedEvent(const MappedEvent::ptr& mappedEvent) {
    m_eventHandler->addMappedEvent(mappedEvent);
}


/**
 * Show an info message on the screen
 */
void Plugin::raiseInfoMessage(std::string_view text) {
    LOG_INFO << text << LOG_END
    m_messages->addMessage(MessageType::Info, text);
}


/**
 * Show an error message on the screen
 */
void Plugin::raiseErrorMessage(std::string_view text) {
    LOG_ERROR << text << LOG_END
    m_messages->addMessage(MessageType::Error, text);
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
    LOG_INFO << "PLUGIN :: Initialise MIDI devices" << LOG_END

    // close open connections and clear device list
    closeMidiConnections();

    m_profile.createMidiDevices(m_devices);

    if (m_devices->size() > 0)
        m_devices->openConnections();
}


/**
 * Close all open midi connections
 */
void Plugin::closeMidiConnections() {

}


/**
 * Destroy device list
 */
void Plugin::destroyDeviceList() {
    LOG_INFO << "PLUGIN :: Destroy MIDI device list" << LOG_END

    m_devices->clear();
}


/**
 * Display messages in the message queue
 */
void Plugin::showMessages() {
    if (m_messages->size() > 0)
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
            window = std::make_shared<MessageWindow>(m_messages);
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