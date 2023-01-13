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

#include "settings.h"

// Standard
#include <filesystem>

// XMidiCtrl
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
settings::settings(text_logger &in_text_log, environment &in_env)
    : config(in_env),
      m_text_log(in_text_log)
{
    // build name for general settings file
    if (load_file(m_text_log, get_settings_filename())) {
        m_debug_mode = toml::find_or<bool>(m_config, CFG_KEY_DEBUG_MODE, false);
        m_log_midi = toml::find_or<bool>(m_config, CFG_KEY_LOG_MIDI, true);
        m_show_messages = toml::find_or<bool>(m_config, CFG_KEY_SHOW_MSG_DIALOG, true);

        m_max_text_messages = toml::find_or<int>(m_config, CFG_KEY_MAX_TEXT_MESSAGES, 1500);
        m_max_midi_messages = toml::find_or<int>(m_config, CFG_KEY_MAX_MIDI_MESSAGES, 150);

        m_note_name = static_cast<note_name_type>(toml::find_or<int>(m_config, CFG_KEY_NOTE_NAME, 0));

        m_default_text_sort = static_cast<sort_mode>(toml::find_or<int>(m_config, CFG_KEY_DEFAULT_TEXT_SORT, 0));
        m_default_midi_sort = static_cast<sort_mode>(toml::find_or<int>(m_config, CFG_KEY_DEFAULT_MIDI_SORT, 0));

        m_use_common_profile = toml::find_or<bool>(m_config, CFG_KEY_COMMON_PROFILE, true);

        m_info_disabled = toml::find_or<bool>(m_config, CFG_KEY_INFO_DISABLED, false);
        m_info_position = static_cast<window_position>(toml::find_or<int>(m_config, CFG_KEY_INFO_POSITION, 1));
        m_info_offset_x = toml::find_or<int>(m_config, CFG_KEY_INFO_OFFSET_X, 50);
        m_info_offset_y = toml::find_or<int>(m_config, CFG_KEY_INFO_OFFSET_Y, 50);
        m_info_seconds = toml::find_or<int>(m_config, CFG_KEY_INFO_SECONDS, 3);
    } else {
        m_text_log.warn(" --> Will use default settings");
    }
}


/**
 * Destructor
 */
settings::~settings()
{
    close_file(m_text_log);
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Set if debug mode is on
 */
void settings::set_debug_mode(bool in_enabled)
{
    m_debug_mode = in_enabled;
    m_text_log.set_debug_mode(in_enabled);
}


/**
 * Return if debug mode is on
 */
bool settings::debug_mode() const
{
    return m_debug_mode;
}


/**
 * Set if midi logging is active
 */
void settings::set_log_midi(bool in_enabled)
{
    m_log_midi = in_enabled;
}


/**
 * Return if midi logging is active
 */
bool settings::log_midi() const
{
    return m_log_midi;
}


/**
 * Set if the messages dialog should be displayed in case of error
 */
void settings::set_show_messages(const bool in_enabled)
{
    m_show_messages = in_enabled;
}


/**
 * Return if the messages dialog should be displayed in case of error
 */
bool settings::show_messages() const
{
    return m_show_messages;
}


/**
 * Sets the max number of text messages to be logged
 */
void settings::set_max_text_messages(int in_number)
{
    m_max_text_messages = in_number;
    m_text_log.set_max_size(in_number);
}


/**
 * Return the max number of text messages to be logged
 */
int settings::max_text_messages() const
{
    return m_max_text_messages;
}


/**
 * Sets the max number of midi messages to be logged
 */
void settings::set_max_midi_messages(int in_number)
{
    m_max_midi_messages = in_number;
}


/**
 * Return the max number of midi messages to be logged
 */
int settings::max_midi_messages() const
{
    return m_max_midi_messages;
}


/**
 * Set the default sort mode for text messages
 */
void settings::set_default_text_sort(sort_mode in_mode)
{
    m_default_text_sort = in_mode;
}


/**
 * Return the default sort mode for text messages
 */
sort_mode settings::default_text_sort() const
{
    return m_default_text_sort;
}


/**
 * Set the default sort mode for MIDI messages
 */
void settings::set_default_midi_sort(sort_mode in_mode)
{
    m_default_midi_sort = in_mode;
}


/**
 * Return the default sort mode for MIDI messages
 */
sort_mode settings::default_midi_sort() const
{
    return m_default_midi_sort;
}


/**
 * Set the note name type
 */
void settings::set_note_name(note_name_type in_type)
{
    m_note_name = in_type;
}


/**
 * Return the note name type
 */
note_name_type settings::note_name() const
{
    return m_note_name;
}


/**
 * Sets if the common profile is enabled
 */
void settings::set_use_common_profile(bool in_enabled)
{
    m_use_common_profile = in_enabled;
}


/**
 * Return if the common profile is enabled
 */
bool settings::use_common_profile() const
{
    return m_use_common_profile;
}


/**
 * Set if the info window is disabled
 */
void settings::set_info_disabled(bool in_disabled)
{
    m_info_disabled = in_disabled;
}


/**
 * Return if the info window is disabled
 */
bool settings::info_disabled() const
{
    return m_info_disabled;
}


/**
 * Set the position of the info window
 */
void settings::set_info_position(window_position in_position)
{
    m_info_position = in_position;
}


/**
 * Return the position of the info window
 */
window_position settings::info_position() const
{
    return m_info_position;
}


/**
 * Set the info window offset x
 */
void settings::set_info_offset_x(int in_offset)
{
    m_info_offset_x = in_offset;
}


/**
 * Return the info window offset x
 */
int settings::info_offset_x() const
{
    return m_info_offset_x;
}


/**
 * Set the info window offset y
 */
void settings::set_info_offset_y(int in_offset)
{
    m_info_offset_y = in_offset;
}


/**
 * Return the info window offset y
 */
int settings::info_offset_y() const
{
    return m_info_offset_y;
}


/**
 * Set the number of seconds an info message should be displayed
 */
void settings::set_info_seconds(int in_seconds)
{
    m_info_seconds = in_seconds;
}


/**
 * Return the number of seconds an info message should be displayed
 */
int settings::info_seconds() const
{
    return m_info_seconds;
}


/**
 * Save the current settings
 */
void settings::save_settings()
{
    m_config[CFG_KEY_DEBUG_MODE] = m_debug_mode;
    m_config[CFG_KEY_LOG_MIDI] = m_log_midi;
    m_config[CFG_KEY_SHOW_MSG_DIALOG] = m_show_messages;

    m_config[CFG_KEY_MAX_TEXT_MESSAGES] = m_max_text_messages;
    m_config[CFG_KEY_MAX_MIDI_MESSAGES] = m_max_midi_messages;

    m_config[CFG_KEY_NOTE_NAME] = static_cast<int>(m_note_name);

    m_config[CFG_KEY_DEFAULT_TEXT_SORT] = static_cast<int>(m_default_text_sort);
    m_config[CFG_KEY_DEFAULT_MIDI_SORT] = static_cast<int>(m_default_midi_sort);

    m_config[CFG_KEY_COMMON_PROFILE] = m_use_common_profile;

    m_config[CFG_KEY_INFO_DISABLED] = m_info_disabled;
    m_config[CFG_KEY_INFO_POSITION] = static_cast<int>(m_info_position);
    m_config[CFG_KEY_INFO_OFFSET_X] = m_info_offset_x;
    m_config[CFG_KEY_INFO_OFFSET_Y] = m_info_offset_y;
    m_config[CFG_KEY_INFO_SECONDS] = m_info_seconds;

    std::ofstream stream;
    std::string filename = get_settings_filename();

    if (filename.empty()) {
        m_text_log.error("Could not determine file name for general settings");
        return;
    }

    // check if our directory already exists in the preference folder
    if (!utils::create_preference_folders(m_text_log, env()))
        return;

    stream.open(filename, std::ios_base::out | std::ios_base::trunc);

    if (!stream.is_open()) {
        m_text_log.error("Could not save general settings file '" + filename + "'");
        return;
    }

    stream << m_config;
    stream.close();
}


/**
 * Returns the file name for the general settings
 */
std::string settings::get_settings_filename()
{
    std::string filename = env().preferences_path().string() + std::string(XMIDICTRL_NAME) +
                           std::string(SETTINGS_FILE_SUFFIX);

    return filename;
}

} // Namespace xmidictrl