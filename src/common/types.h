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

// MIDI unsigned int none
const unsigned char MIDI_NONE(255);

const unsigned char MIDI_VELOCITY_MIN(0);

const unsigned char MIDI_VELOCITY_MAX(127);

// Interval of flight loop
const float FLIGHTLOOP_INTERVAL(-1.0f);

// Name of the aircraft profile file
const char *const FILENAME_PROFILE = "XMidiCtrl.toml";

// Suffix for the XMidiCtrl log file
const char *const LOGFILE_SUFFIX = "_Log.txt";

// Suffix the general settings file
const char *const SETTINGS_FILE_SUFFIX = "_Settings.toml";

// Profiles directory name
const char *const PROFILES_DIRECTORY_NAME = "Profiles";

// Keys for mappings and MIDI message types
const char *const KEY_CONTROL_CHANGE = "CC";
const char *const KEY_NOTE = "NOTE";
const char *const KEY_PITCH_BEND = "PITCH";
const char *const KEY_PROGRAM_CHANGE = "PRG";

// Menu items
const char *const MENUITEM_ABOUT_WINDOW = "ABOUT_WINDOW";
const char *const MENUITEM_RELOAD_PROFILE = "RELOAD_PROFILE";
const char *const MENUITEM_MESSAGES_WINDOW = "MESSAGES_WINDOW";
const char *const MENUITEM_DEVICES_WINDOW = "DEVICES_WINDOW";
const char *const MENUITEM_SETTINGS_WINDOW = "SETTINGS_WINDOW";
const char *const MENUITEM_SHOW_DOCUMENTATION = "SHOW_DOCUMENTATION";
const char *const MENUITEM_SHOW_SUPPORT = "SHOW_SUPPORT";
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
const char *const CFG_KEY_COMMON_PROFILE = "common_profile";
const char *const CFG_KEY_DATAREF = "dataref";
const char *const CFG_KEY_DATAREF_PUSH = "dataref_push";
const char *const CFG_KEY_DATAREF_PULL = "dataref_pull";
const char *const CFG_KEY_DEBUG_MODE = "debug_mode";
const char *const CFG_KEY_DEFAULT_MIDI_SORT = "default_midi_sort";
const char *const CFG_KEY_DEFAULT_TEXT_SORT = "default_text_sort";
const char *const CFG_KEY_DEVICE = "device";
const char *const CFG_KEY_ENCODER_MODE = "encoder_mode";
const char *const CFG_KEY_LABEL = "label";
const char *const CFG_KEY_LOG_MIDI = "log_midi";
const char *const CFG_KEY_MAPPING = "mapping";
const char *const CFG_KEY_MAPPING_IN = "mapping_in";
const char *const CFG_KEY_MAPPING_INIT = "mapping_init";
const char *const CFG_KEY_MAPPING_OUT = "mapping_out";
const char *const CFG_KEY_MAX_MIDI_MESSAGES = "max_midi_messages";
const char *const CFG_KEY_MAX_TEXT_MESSAGES = "max_text_messages";
const char *const CFG_KEY_MODE = "mode";
const char *const CFG_KEY_MODE_OUT = "mode_out";
const char *const CFG_KEY_MODIFIER_DOWN = "modifier_down";
const char *const CFG_KEY_MODIFIER_FAST_DOWN = "modifier_fast_down";
const char *const CFG_KEY_MODIFIER_FAST_UP = "modifier_fast_up";
const char *const CFG_KEY_MODIFIER_UP = "modifier_up";
const char *const CFG_KEY_NAME = "name";
const char *const CFG_KEY_NOTE = "note";
const char *const CFG_KEY_NOTE_NAME = "note";
const char *const CFG_KEY_PITCH_BEND = "pitch";
const char *const CFG_KEY_PROGRAM_CHANGE = "prg";
const char *const CFG_KEY_PORT_IN = "port_in";
const char *const CFG_KEY_PORT_OUT = "port_out";
const char *const CFG_KEY_SEND_ON = "send_on";
const char *const CFG_KEY_SEND_OFF = "send_off";
const char *const CFG_KEY_SHOW_MSG_DIALOG = "show_message_dialog";
const char *const CFG_KEY_SL = "sl";
const char *const CFG_KEY_SL_DATAREF = "sl_dataref";
const char *const CFG_KEY_TEXT = "text";
const char *const CFG_KEY_TITLE = "title";
const char *const CFG_KEY_TYPE = "type";
const char *const CFG_KEY_VALUE = "value";
const char *const CFG_KEY_VALUES = "values";
const char *const CFG_KEY_VALUES_PUSH = "values_push";
const char *const CFG_KEY_VALUES_PULL = "values_pull";
const char *const CFG_KEY_VALUE_MIN = "value_min";
const char *const CFG_KEY_VALUE_MAX = "value_max";
const char *const CFG_KEY_VALUE_ON = "value_on";
const char *const CFG_KEY_VALUE_OFF = "value_off";
const char *const CFG_KEY_VELOCITY = "velocity";
const char *const CFG_KEY_VELOCITY_ON = "velocity_on";
const char *const CFG_KEY_VELOCITY_OFF = "velocity_off";
const char *const CFG_KEY_VERSION = "version";
const char *const CFG_KEY_INFO_DISABLED = "info_disabled";
const char *const CFG_KEY_INFO_OFFSET_X = "info_offset_x";
const char *const CFG_KEY_INFO_OFFSET_Y = "info_offset_y";
const char *const CFG_KEY_INFO_POSITION = "info_position";
const char *const CFG_KEY_INFO_SECONDS = "info_seconds";

// Mapping Types as strings
const char *const CFG_MAPTYPE_COMMAND = "cmd";
const char *const CFG_MAPTYPE_DATAREF = "drf";
const char *const CFG_MAPTYPE_ENCODER = "enc";
const char *const CFG_MAPTYPE_PUSH_PULL = "pnp";
const char *const CFG_MAPTYPE_SLIDER = "sld";




//---------------------------------------------------------------------------------------------------------------------
//   ENUMERATIONS
//---------------------------------------------------------------------------------------------------------------------

// time point
using time_point = std::chrono::system_clock::time_point;

// Log levels
enum class log_level {
    error,
    warn,
    info,
    debug
};

// Note names
enum class note_name_type {
    sharp = 0,
    flat  = 1
};

// MIDI message type
enum class midi_msg_type {
    aftertouch,
    channel_pressure,
    control_change,
    note_off,
    note_on,
    pitch_bend,
    program_change,
    none
};

// MIDI mapping types
enum class map_type {
    none,
    command,
    dataref,
    encoder,
    push_pull,
    slider
};

// Mapping data type
enum class map_data_type {
    none,
    control_change,
    note,
    pitch_bend,
    program_change
};

// Send mode outbound
enum class mode_out {
    on_change,
    permanent
};

// Dataref mode
enum class dataref_mode {
    toggle,
    momentary
};

// Encoder mode
enum class encoder_mode {
    relative,
    range
};

// Send mode
enum class send_mode {
    all,
    one
};

// Sorting mode
enum class sort_mode {
    ascending  = 0,
    descending = 1
};

// Window position
enum class window_position {
    top_left      = 0,
    bottom_left   = 1,
    top_right     = 2,
    bottom_right  = 3,
    center        = 4,
    top_center    = 5,
    bottom_center = 6
};

// Window types
enum class window_type {
    about_window,
    messages_window,
    devices_window,
    info_window,
    profile_window,
    settings_window,
};

} // Namespace xmidictrl

#endif // TYPES_H
