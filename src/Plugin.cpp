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
#include "PluginLogger.h"

// XMidiCtrl
#include "Config.h"
#include "Plugin.h"
#include "ui/AboutDialog.h"
#include "ui/MidiDevicesDialog.h"

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
    LOG_METHOD_START

    // initialize
    m_flightLoopId = nullptr;

    LOG_METHOD_END
}


/**
 * Destructor
 */
Plugin::~Plugin() {
    LOG_METHOD_START

    m_flightLoopId = nullptr;

    // close and destroy all midi connections and devices
    closeMidiConnections();
    destroyDeviceList();

    LOG_METHOD_END
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
float Plugin::callbackFlightLoop(float elapsedMe, float elapsedSim, int counter, void* refcon) {
    LOG_METHOD_START

    // call instance method
    if (refcon != nullptr) {
        auto midiCtrl = static_cast< Plugin* >(refcon);
        midiCtrl->processFlightLoop(elapsedMe, elapsedSim, counter);
    }

    LOG_METHOD_END
    return XMIDICTRL_FLIGHTLOOP_INTERVAL;
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
    LOG_METHOD_START
    LOG_INFO << "PLUGIN :: Enable plugin" << LOG_END

    // create all required menu entries
    m_menu.createMenu();

    // initialise configured midi devices
    initialiseDevices();

    // register flight loop
    XPLMCreateFlightLoop_t params = { sizeof(XPLMCreateFlightLoop_t),
                                      xplm_FlightLoop_Phase_BeforeFlightModel,
                                      callbackFlightLoop, this };
    m_flightLoopId = XPLMCreateFlightLoop(&params);

    if (m_flightLoopId != nullptr) {
        LOG_INFO << "PLUGIN :: Flight loop successfully registered" << LOG_END

        XPLMScheduleFlightLoop(m_flightLoopId, XMIDICTRL_FLIGHTLOOP_INTERVAL, true);
    } else {
        LOG_ERROR << "PLUGIN :: Could not create flight loop" << LOG_END
    }

    LOG_METHOD_END
}


/**
 * Disable the plugin
 */
void Plugin::disablePlugin() {
    LOG_METHOD_START

    // delete the menu entries
    m_menu.deleteMenu();

    // destroy the flight loop
    if (m_flightLoopId != nullptr) {
        XPLMDestroyFlightLoop(m_flightLoopId);
        m_flightLoopId = nullptr;

        LOG_INFO << "PLUGIN :: Flight loop destroyed" << LOG_END
    }

    // close midi connections
    closeMidiConnections();    

    // destroy devices
    destroyDeviceList();

    LOG_INFO << "PLUGIN :: Disabled plugin" << LOG_END
    LOG_METHOD_END
}


/**
 * Reload the settings
 */
void Plugin::reloadAircraftSettings() {
    LOG_METHOD_START
    LOG_INFO << "PLUGIN :: Reload settings" << LOG_END

    // reload settings for current aircraft
    m_settings.reloadSettingsForAircraft();

    // initialise configured midi devices
    initialiseDevices();

    LOG_METHOD_END
}


/**
 * Add a midi event to the queue
 */
void Plugin::addMidiEvent(const std::shared_ptr<MidiEvent>& midiEvent) {
    LOG_METHOD_START

    m_eventHandler.addMidiEvent(midiEvent);

    LOG_METHOD_END
}


/**
 * Search for available midi devices
 */
void Plugin::showMidiDevicesDialog() {
    LOG_METHOD_START

    auto dialog = new MidiDevicesDialog();
    dialog->show();

    LOG_METHOD_END
}


/**
 * Show the about dialog
 */
void Plugin::showAboutDialog() {
    LOG_METHOD_START

    auto dialog = new AboutDialog();
    dialog->show();

    LOG_METHOD_END
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Initialise configured midi devices
 */
void Plugin::initialiseDevices() {
    LOG_METHOD_START
    LOG_INFO << "PLUGIN :: Initialise MIDI devices" << LOG_END

    std::vector<DeviceSettings> deviceList;
    std::shared_ptr<Device> midiDevice;

    // close open connections and clear device list
    closeMidiConnections();

    deviceList = m_settings.deviceList();

    for (const auto& deviceSettings: deviceList) {
        // create midi device
        midiDevice = std::make_shared<Device>(deviceSettings);

        m_midiDevices.emplace(midiDevice->name(), midiDevice);

        // open inbound and outbound port
        midiDevice->openConnections();
    }

    LOG_METHOD_END
}


/**
 * Close all open midi connections
 */
void Plugin::closeMidiConnections() {
    LOG_METHOD_START
    LOG_INFO << "PLUGIN :: Close MIDI connections" << LOG_END

    for (auto const& device : m_midiDevices) {
        if (device.second != nullptr) {
            device.second->closeConnections();    
        }
    }

    m_midiDevices.clear();

    LOG_METHOD_END
}    


/**
 * Destroy device list
 */
void Plugin::destroyDeviceList() {
    LOG_METHOD_START
    LOG_INFO << "PLUGIN :: Destroy MIDI device list" << LOG_END

    m_midiDevices.clear();

    LOG_METHOD_END
}

} // Namespace XMidiCtrl