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
#include <string_view>

// X-Plane SDK Utils
#include "PluginLogger.h"

// XMidiCtrl
#include "Global.h"
#include "Plugin.h"


using namespace XMidiCtrl;


//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESCTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Plugin::Plugin()
    : XPlanePlugin(XMIDICTRL_NAME, XMIDICTRL_VERSION), m_eventHandler(this->environment()) {
    // initialise
    m_flightLoopId = nullptr;
}


/**
 * Destructor
 */
Plugin::~Plugin() {
    m_flightLoopId = nullptr;

    // close and destroy all midi devices
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
float Plugin::callbackFlightLoop(float elapsedMe, float elapsedSim, int counter, void * refcon) {
    Plugin* midiCtrl = nullptr;
    
    // call instance method
    if (refcon != nullptr) {
        midiCtrl = static_cast< Plugin* >(refcon);
        midiCtrl->processFlightLoop(elapsedMe, elapsedSim, counter);
    }

    return XMIDICTRL_FLIGHTLOOP_INTERVAL;
}


/**
 * Process the flight loop
 */
void Plugin::processFlightLoop(float elapsedMe, float elapsedSim, int counter) {
    // process messages
    m_eventHandler.processEvents();
}


/**
 * Enable the plugin
 */
void Plugin::enablePlugin() {
    LOG_INFO << "PLUGIN :: Enable plugin" << LOG_END

    // create all required menu entries
    m_pluginMenu.createMenu();

    // probe for available midi ports
    //probeMidiPorts();

    // initialise configured midi devices
    initialiseDevices();

    // register flight loop
    XPLMCreateFlightLoop_t params = {sizeof(XPLMCreateFlightLoop_t), xplm_FlightLoop_Phase_BeforeFlightModel, callbackFlightLoop, this };
    m_flightLoopId = XPLMCreateFlightLoop(&params);

    if (m_flightLoopId != nullptr) {
        LOG_INFO << "PLUGIN :: Flight loop successfully registered" << LOG_END

        XPLMScheduleFlightLoop(m_flightLoopId, XMIDICTRL_FLIGHTLOOP_INTERVAL, true);
    }
}


/**
 * Disable the plugin
 */
void Plugin::disablePlugin() {
    // delete the menu entries
    m_pluginMenu.deleteMenu();

    // destroy the flight loop
    if (m_flightLoopId != nullptr) {
        XPLMDestroyFlightLoop(m_flightLoopId);
        m_flightLoopId = nullptr;

        LOG_INFO <<"PLUGIN :: Flight loop destroyed" << LOG_END
    }

    // close midi connections
    closeMidiConnections();    

    // destroy devices
    destroyDeviceList();

    LOG_INFO << "PLUGIN :: Disabled plugin" << LOG_END
}


/**
 * Reload the settings
 */
void Plugin::reloadSettings() {
    LOG_INFO << "PLUGIN :: Reload settings" << LOG_END

    // reload settings for current aircraft
    m_settings.reloadSettingsForAircraft();

    // initialise configured midi devices
    initialiseDevices();
}


/**
 * Add a midi event to the queue
 */
void Plugin::addMidiEvent(std::shared_ptr<MidiEvent> midiEvent) {
    m_eventHandler.addMidiEvent(midiEvent);
}


/**
 * Show the about dialog
 */
void Plugin::showAboutDialog() {
}


/**
 * Probe for available midi devices
 */
void Plugin::probeMidiPorts() {
    RtMidiIn* midiIn = 0;

    // Construct midi event listener
    try {
        midiIn = new RtMidiIn();
    }
    catch ( RtMidiError& midiError) {
        LOG_ERROR << midiError.what() << LOG_END
    }

    // Check input ports
    unsigned int portCount = midiIn->getPortCount(); 
    std::string portName;

    //pluginLog << "No of Ports found: " << portCount << std::endl;

    for (unsigned int i = 0; i < portCount; i++) {
        try {
            portName = midiIn->getPortName(i);

            //pluginLog << "Port: " << portName << std::endl;
        }
        catch ( RtMidiError& midiError) {
            LOG_ERROR << midiError.what() << LOG_END
        }
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Initialise configured midi devices
 */
void Plugin::initialiseDevices() {
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
}


/**
 * Close all open midi connections
 */
void Plugin::closeMidiConnections() {
    LOG_INFO << "PLUGIN :: Close MIDI connections" << LOG_END

    for (auto const& device : m_midiDevices) {
        if (device.second != nullptr) {
            device.second->closeConnections();    
        }
    }

    m_midiDevices.clear();
}    


/**
 * Destroy device list
 */
void Plugin::destroyDeviceList() {
    LOG_DEBUG << "PLUGIN :: Destroy MIDI device list" << LOG_END

    //for (auto device : m_midiDevices) {
    //    delete device.second;
    //    device.second = nullptr;
    //}

    m_midiDevices.clear();
}

