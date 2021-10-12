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

// X-Plane Environment
#include "utils/Logger.h"

// XMidiCtrl
#include "Config.h"
#include "Plugin.h"
#include "ui/AboutDialog.h"
#include "ui/MidiDevicesDialog.h"
#include "ui/SettingsDialog.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Plugin::Plugin()
        : XPlanePlugin(XMIDICTRL_NAME, XMIDICTRL_VERSION_STR),
          m_eventHandler(this->environment()) {
    // initialize
    m_flightLoopId = nullptr;

    // create device list
    m_devices = std::make_shared<DeviceList>();
}


/**
 * Destructor
 */
Plugin::~Plugin() {
    m_flightLoopId = nullptr;

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
Plugin &Plugin::Instance() {
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
    m_eventHandler.processEvents();
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
void Plugin::addMappedEvent(MappedEvent::ptr mappedEvent) {
    m_eventHandler.addMappedEvent(mappedEvent);
}


/**
 * Search for available midi devices
 */
void Plugin::showMidiDevicesDialog() {
    showWindow(WINDOW_DEVICES);
}


/**
 * Show general settings dialog
 */
void Plugin::showSettingsDialog() {
    showWindow(WINDOW_SETTINGS);
}

/**
 * Show the about dialog
 */
void Plugin::showAboutDialog() {
    showWindow(WINDOW_ABOUT);
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create and returns windows
 */
std::shared_ptr<XPlaneWindow> Plugin::createWindow(std::string_view windowId) {
    std::shared_ptr<XPlaneWindow> window;

    if (windowId == WINDOW_ABOUT)
        window = std::make_shared<AboutDialog>();
    else if (windowId == WINDOW_DEVICES)
        window = std::make_shared<MidiDevicesDialog>();
    else if (windowId == WINDOW_SETTINGS)
        window = std::make_shared<SettingsDialog>();

    return window;
}


/**
 * Initialise configured midi devices
 */
void Plugin::initialiseDevices() {
    LOG_INFO << "PLUGIN :: Initialise MIDI devices" << LOG_END

    //std::vector<DeviceSettings> deviceList;
    //std::shared_ptr<Device> midiDevice;

    // close open connections and clear device list
    closeMidiConnections();

    m_profile.createMidiDevices(m_devices);

    /*
    deviceList = m_profile.deviceList();

    for (const auto &deviceSettings: deviceList) {
        // create midi device
        midiDevice = std::make_shared<Device>(deviceSettings);

        m_midiDevices.emplace(midiDevice->name(), midiDevice);

        // open inbound and outbound port
        midiDevice->openConnections();
    }
    */
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

} // Namespace XMidiCtrl