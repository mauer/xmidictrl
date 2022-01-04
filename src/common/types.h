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

#ifndef TYPES_H
#define TYPES_H

// Standard
#include <chrono>
#include <map>
#include <string>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTANTS
//---------------------------------------------------------------------------------------------------------------------

// Colour for Values im ImGui
#define COL_TEXT_VALUE (ImVec4) ImColor(255, 127, 39)

// Interval of flight loop
const int FLIGHTLOOP_INTERVAL(-1.0);

// Offset for midi channel status
const int OFFSET_MIDI_CHANNEL_STATUS(175);

// Name of the aircraft profile file
const char *const FILENAME_PROFILE = "XMidiCtrl.toml";

// Suffix for the XMidiCtrl log file
const char *const LOGFILE_SUFFIX = "_Log.txt";

// Suffix the general settings file
const char *const SETTINGS_FILE_SUFFIX = "_Settings.toml";

// Profiles directory name
const char *const PROFILES_DIRECTORY_NAME = "Profiles";

// Menu items
const char *const MENUITEM_ABOUT_WINDOW = "ABOUT_WINDOW";
const char *const MENUITEM_RELOAD_PROFILE = "RELOAD_PROFILE";
const char *const MENUITEM_MESSAGES_WINDOW = "MESSAGES_WINDOW";
const char *const MENUITEM_DEVICES_WINDOW = "DEVICES_WINDOW";
const char *const MENUITEM_SETTINGS_WINDOW = "SETTINGS_WINDOW";
const char *const MENUITEM_SHOW_DOCUMENTATION = "SHOW_DOCUMENTATION";
const char *const MENUITEM_PROFILE_WINDOW = "PROFILE_WINDOW";

// Custom commands
const char *const COMMAND_MESSAGE_WINDOW = "MESSAGES_WINDOW";
const char *const COMMAND_PROFILE_WINDOW = "PROFILE_WINDOW";
const char *const COMMAND_RELOAD_PROFILE = "RELOAD_PROFILE";
const char *const COMMAND_TOGGLE_SUBLAYER = "TOGGLE_SUBLAYER";

// Keys for the config files
const char *const CFG_KEY_CC = "cc";
const char *const CFG_KEY_CC_DEPRECATED = "CC";
const char *const CFG_KEY_CH = "ch";
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
const char *const CFG_KEY_LOG_LEVEL = "logging_level";
const char *const CFG_KEY_LOG_MIDI = "log_midi";
const char *const CFG_KEY_MAPPING = "mapping";
const char *const CFG_KEY_MAPPING_IN = "mapping_in";
const char *const CFG_KEY_MAPPING_OUT = "mapping_out";
const char *const CFG_KEY_MODE_OUT = "mode_out";
const char *const CFG_KEY_MODIFIER_DOWN = "modifier_down";
const char *const CFG_KEY_MODIFIER_FAST_DOWN = "modifier_fast_down";
const char *const CFG_KEY_MODIFIER_FAST_UP = "modifier_fast_up";
const char *const CFG_KEY_MODIFIER_UP = "modifier_up";
const char *const CFG_KEY_NAME = "name";
const char *const CFG_KEY_PORT_IN = "port_in";
const char *const CFG_KEY_PORT_OUT = "port_out";
const char *const CFG_KEY_SHOW_MSG_DIALOG = "show_message_dialog";
const char *const CFG_KEY_SL = "sl";
const char *const CFG_KEY_SL_DATAREF = "sl_dataref";
const char *const CFG_KEY_TITLE = "title";
const char *const CFG_KEY_TYPE = "type";
const char *const CFG_KEY_VALUES = "values";
const char *const CFG_KEY_VALUE_MIN = "value_min";
const char *const CFG_KEY_VALUE_MAX = "value_max";
const char *const CFG_KEY_VALUE_ON = "value_on";
const char *const CFG_KEY_VALUE_OFF = "value_off";
const char *const CFG_KEY_VELOCITY_ON = "velocity_on";
const char *const CFG_KEY_VELOCITY_OFF = "velocity_off";
const char *const CFG_KEY_VERSION = "version";

// Mapping Types as strings
const char *const CFG_MAPTYPE_COMMAND = "cmd";
const char *const CFG_MAPTYPE_DATAREF = "drf";
const char *const CFG_MAPTYPE_ENCODER = "enc";
const char *const CFG_MAPTYPE_INTERNAL = "int";
const char *const CFG_MAPTYPE_PUSH_PULL = "pnp";
const char *const CFG_MAPTYPE_SLIDER = "sld";




//---------------------------------------------------------------------------------------------------------------------
//   ENUMERATIONS
//---------------------------------------------------------------------------------------------------------------------

// time point
//using time_point = std::chrono::time_point<std::chrono::system_clock>;
using time_point = std::chrono::system_clock::time_point;

// Log levels
enum class log_level {
    error = 1,
    warn = 2,
    info = 3,
    debug = 4
};

// Message types
enum class text_msg_type {
    all = 0,
    error = 1,
    warn = 2,
    info = 3,
    debug = 4
};

// MIDI type
enum class midi_type {
    inbound,
    outbound
};

// MIDI velocity codes
enum class midi_velocity {
    key_released = 0,
    key_pressed = 127
};

// MIDI mapping types
enum class map_type {
    none,
    command,
    dataref,
    encoder,
    internal,
    push_pull,
    slider
};

// Send mode outbound
enum class mode_out {
    on_change,
    permanent
};

// Window types
enum class window_type {
    about_window,
    messages_window,
    devices_window,
    profile_window,
    settings_window,
};

} // Namespace xmidictrl

#endif // TYPES_H
