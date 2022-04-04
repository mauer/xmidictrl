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
#include <utility>

// XMidiCtrl
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
settings::settings(text_logger &in_text_log, midi_logger &in_midi_log, xplane &in_xp)
    : config(in_xp),
      m_text_log(in_text_log),
      m_midi_log(in_midi_log)
{
    // build name for general settings file
    if (!load_file(m_text_log, get_settings_filename()))
        m_text_log.warn(" --> Will use default settings");
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
void settings::set_debug_mode(bool in_mode)
{
    m_config[CFG_KEY_DEBUG_MODE] = in_mode;
    m_text_log.set_debug_mode(in_mode);
}


/**
 * Return if debug mode is on
 */
bool settings::debug_mode() const
{
    return toml::find_or<bool>(m_config, CFG_KEY_DEBUG_MODE, false);
}


/**
 * Set if midi logging is active
 */
void settings::set_log_midi(bool in_enabled)
{
    m_config[CFG_KEY_LOG_MIDI] = in_enabled;
    m_midi_log.set_state(in_enabled);
}


/**
 * Return if midi logging is active
 */
bool settings::log_midi() const
{
    return toml::find_or<bool>(m_config, CFG_KEY_LOG_MIDI, true);
}


/**
 * Set if the messages dialog should be displayed in case of error
 */
void settings::set_show_messages(const bool in_enabled)
{
    m_config[CFG_KEY_SHOW_MSG_DIALOG] = in_enabled;
}


/**
 * Return if the messages dialog should be displayed in case of error
 */
bool settings::show_messages() const
{
    return toml::find_or<bool>(m_config, CFG_KEY_SHOW_MSG_DIALOG, true);
}


/**
 * Sets the max number of text messages to be logged
 */
void settings::set_max_text_messages(int in_number)
{
    m_config[CFG_KEY_MAX_TEXT_MESSAGES] = in_number;
    m_text_log.set_max_size(in_number);
}


/**
 * Return the max number of text messages to be logged
 */
int settings::max_text_messages() const
{
    return toml::find_or<int>(m_config, CFG_KEY_MAX_TEXT_MESSAGES, 500);
}


/**
 * Sets the max number of midi messages to be logged
 */
void settings::set_max_midi_messages(int in_number)
{
    m_config[CFG_KEY_MAX_MIDI_MESSAGES] = in_number;
    m_text_log.set_max_size(in_number);
}


/**
 * Return the max number of midi messages to be logged
 */
int settings::max_midi_messages() const
{
    return toml::find_or<int>(m_config, CFG_KEY_MAX_MIDI_MESSAGES, 500);
}


/**
 * Sets if the common profile is enabled
 */
void settings::set_common_profile(bool in_enabled)
{
    m_config[CFG_KEY_COMMON_PROFILE] = in_enabled;
}


/**
 * Return if the common profile is enabled
 */
bool settings::common_profile() const
{
    return toml::find_or<int>(m_config, CFG_KEY_COMMON_PROFILE, true);
}


/**
 * Save the current settings
 */
void settings::save_settings()
{
    std::ofstream stream;
    std::string filename = get_settings_filename();

    if (filename.empty()) {
        m_text_log.error("Could not determine file name for general settings");
        return;
    }

    // check if our directory already exists in the preference folder
    if (!conversions::create_preference_folders(m_text_log, xp()))
        return;

    stream.open(filename, std::ios_base::out | std::ios_base::trunc);

    if (!stream.is_open()) {
        m_text_log.error("Could not save general settings file '%s'", filename.c_str());
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
    std::string filename = xp().preferences_path().data() + std::string(XMIDICTRL_NAME) +
                           std::string(SETTINGS_FILE_SUFFIX);

    return filename;
}

} // Namespace xmidictrl