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

#ifndef _TYPES_H_
#define _TYPES_H_

// Standard
#include <map>
#include <string>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTANTS
//---------------------------------------------------------------------------------------------------------------------

// Interval of flight loop
const int FLIGHTLOOP_INTERVAL(-1.0);

// Name of the aircraft profile file
const char *const FILENAME_PROFILE = "XMidiCtrl.toml";

// Suffix for the XMidiCtrl log file
const char *const LOGFILE_SUFFIX = "_Log.txt";

// Suffix the general settings file
const char *const SETTINGS_FILE_SUFFIX = "_Settings.toml";

// Profiles directory name
const char *const PROFILES_DIRECTORY_NAME = "Profiles";

// Menu items
const char *const MENUITEM_ABOUT_DIALOG = "ABOUT_DIALOG";
const char *const MENUITEM_RELOAD_AIRCRAFT_PROFILE = "RELOAD_AIRCRAFT_PROFILE";
const char *const MENUITEM_MESSAGES_DIALOG = "MESSAGES_DIALOG";
const char *const MENUITEM_MIDI_DEVICES_DIALOG = "MIDI_DEVICES_DIALOG";
const char *const MENUITEM_SETTINGS_DIALOG = "SETTINGS_DIALOG";
const char *const MENUITEM_AIRCRAFT_PROFILE_DIALOG = "AIRCRAFT_PROFILE_DIALOG";

// Keys for the config files
const char *const CFG_KEY_CC = "cc";
const char *const CFG_KEY_CC_DEPRECATED = "CC";
const char *const CFG_KEY_COMMAND = "command";
const char *const CFG_KEY_COMMAND_DOWN = "command_down";
const char *const CFG_KEY_COMMAND_FAST_DOWN = "command_fast_down";
const char *const CFG_KEY_COMMAND_FAST_UP = "command_fast_up";
const char *const CFG_KEY_COMMAND_MIDDLE = "command_middle";
const char *const CFG_KEY_COMMAND_PULL = "command_pull";
const char *const CFG_KEY_COMMAND_PUSH = "command_push";
const char *const CFG_KEY_COMMAND_UP = "command_up";
const char *const CFG_KEY_DATAREF = "dataref";
const char *const CFG_KEY_DEVICE = "device";
const char *const CFG_KEY_LOG_LEVEL = "log_level";
const char *const CFG_KEY_MAPPING = "mapping";
const char *const CFG_KEY_MAPPING_IN = "mapping_in";
const char *const CFG_KEY_MAPPING_OUT = "mapping_out";
const char *const CFG_KEY_NAME = "name";
const char *const CFG_KEY_PORT_IN = "port_in";
const char *const CFG_KEY_PORT_OUT = "port_out";
const char *const CFG_KEY_SHOW_MSG_DIALOG = "show_message_dialog";
const char *const CFG_KEY_TYPE = "type";
const char *const CFG_KEY_VALUE_ON = "value_on";
const char *const CFG_KEY_VALUE_OFF = "value_off";
const char *const CFG_KEY_VERSION = "version";

// Mapping Types as strings
const char *const CFG_MAPTYPE_COMMAND = "cmd";
const char *const CFG_MAPTYPE_DATAREF = "drf";
const char *const CFG_MAPTYPE_ENCODER = "enc";
const char *const CFG_MAPTYPE_INTERNAL = "int";
const char *const CFG_MAPTYPE_PUSH_PULL = "pnp";
const char *const CFG_MAPTYPE_SLIDER = "sld";

// Windows Types
const char *const WINDOW_ABOUT = "WINDOW_ABOUT";
const char *const WINDOW_DEVICES = "WINDOW_DEVICES";
const char *const WINDOW_MESSAGES = "WINDOW_MESSAGES";
const char *const WINDOW_SETTINGS = "WINDOW_SETTINGS";




//---------------------------------------------------------------------------------------------------------------------
//   ENUMERATIONS
//---------------------------------------------------------------------------------------------------------------------

// Log levels
enum class log_level {
    error = 1,
    warn = 2,
    info = 3,
    debug = 4
};

// Message types
enum class message_type {
    all = 0,
    error = 1,
    warn = 2,
    info = 3,
    debug = 4
};

// Midi mapping types
enum class mapping_type {
    none,
    command,
    dataref,
    encoder,
    internal,
    push_pull,
    slider
};

// Midi status codes
enum class midi_status {
    note_begin = 154,
    note_end = 138,
    control_change = 186
};

// Midi velocity codes
enum class midi_velocity {
    key_released = 0,
    key_pressed = 127
};

// Window types
enum class WindowType {
    AboutDialog,
    MessagesDialog,
    MidiDevicesDialog,
    SettingsDialog,
};

} // Namespace xmidictrl

#endif // _TYPES_H_
