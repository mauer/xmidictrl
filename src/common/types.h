//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

#ifndef XMC_TYPES_H
#define XMC_TYPES_H

// Standard
#include <chrono>
#include <map>
#include <string>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTANTS
//---------------------------------------------------------------------------------------------------------------------

// Spacer 2
#define UI_SPACER_2 "  "

// Spacer 3
#define UI_SPACER_3 "   "

// MIDI unsigned int none
const char MIDI_NONE(-1);

const char MIDI_DATA_2_MIN(0);

const char MIDI_DATA_2_MAX(127);

// Interval of flight loop
const float FLIGHTLOOP_INTERVAL(-1.0f);

// Name of the aircraft profile file
const char* const FILENAME_PROFILE = "XMidiCtrl.toml";

// Suffix for the XMidiCtrl log file
const char* const LOGFILE_SUFFIX = "_Log.txt";

// Suffix the general settings file
const char* const SETTINGS_FILE_SUFFIX = "_Settings.toml";

// Suffix for include files
const char* const INCLUDE_FILE_SUFFIX = ".toml";

// Profiles directory name
const char* const PROFILES_DIRECTORY_NAME = "Profiles";

// Includes directory name
const char* const INCLUDES_DIRECTORY_NAME = "Includes";

// Keys for mappings and MIDI message types
const char* const KEY_CONTROL_CHANGE = "CC";
const char* const KEY_NOTE = "NOTE";
const char* const KEY_PITCH_BEND = "PITCH";
const char* const KEY_PROGRAM_CHANGE = "PRG";

// Custom commands
const char* const COMMAND_LOG_VIEWER = "LOG_VIEWER";
const char* const COMMAND_MIDI_WATCHER = "MIDI_WATCHER";
const char* const COMMAND_PROFILE_WINDOW = "PROFILE_WINDOW";
const char* const COMMAND_RELOAD_PROFILE = "RELOAD_PROFILE";
const char* const COMMAND_TOGGLE_SUBLAYER = "TOGGLE_SUBLAYER";

// Keys for the config files
const char* const CFG_KEY_COMMON_PROFILE = "common_profile";
const char* const CFG_KEY_DEBUG_MODE = "debug_mode";
const char* const CFG_KEY_DEFAULT_MIDI_SORT = "default_midi_sort";
const char* const CFG_KEY_DEFAULT_TEXT_SORT = "default_text_sort";
const char* const CFG_KEY_LOG_MIDI = "log_midi";
const char* const CFG_KEY_NOTE_NAME = "note";
const char* const CFG_KEY_SHOW_ERRORS = "show_message_dialog";
const char* const CFG_KEY_VIRTUAL_CHANNEL = "virtual_channel";
const char* const CFG_KEY_INFO_DISABLED = "info_disabled";
const char* const CFG_KEY_INFO_OFFSET_X = "info_offset_x";
const char* const CFG_KEY_INFO_OFFSET_Y = "info_offset_y";
const char* const CFG_KEY_INFO_POSITION = "info_position";
const char* const CFG_KEY_INFO_SECONDS = "info_seconds";

// Mapping Types as strings
const char* const CFG_MAPTYPE_COMMAND = "cmd";
const char* const CFG_MAPTYPE_CONSTANT = "con";
const char* const CFG_MAPTYPE_DATAREF = "drf";
const char* const CFG_MAPTYPE_ENCODER = "enc";
const char* const CFG_MAPTYPE_PUSH_PULL = "pnp";
const char* const CFG_MAPTYPE_SLIDER = "sld";




//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

// time point
using time_point = std::chrono::system_clock::time_point;




//---------------------------------------------------------------------------------------------------------------------
//   ENUMERATIONS
//---------------------------------------------------------------------------------------------------------------------

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
    flat = 1
};

// Dataref mode
enum class dataref_mode {
    toggle,
    momentary
};

// Send mode
enum class send_mode {
    all,
    one
};

// Sorting mode
enum class sort_mode {
    ascending = 0,
    descending = 1
};

// Window position
enum class window_position {
    top_left = 0,
    bottom_left = 1,
    top_right = 2,
    bottom_right = 3,
    center = 4,
    top_center = 5,
    bottom_center = 6
};

// Window types
enum class window_type {
    about_window,
    devices_window,
    log_viewer,
    info_window,
    midi_watcher,
    profile_window,
    settings_window,
};

} // Namespace xmidictrl

#endif // XMC_TYPES_H
