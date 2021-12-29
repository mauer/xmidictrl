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

#include "utils.h"

// Standard
#include <ctime>
#include <filesystem>
#include <utility>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check if the given key is in the config
 */
bool utils::toml_contains(toml::value &settings, std::string_view name, bool mandatory)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_contains --> name is empty" << LOG_END
        return false;
    }

    try {
        // read dataref
        if (settings.contains(name.data())) {
            return true;
        } else {
            if (mandatory) {
                LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str()
                          << LOG_END
                LOG_ERROR << " --> Parameter '" << name << "' not found" << LOG_END
            }

            return false;
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return false;
}


/**
 * Read the value of a string parameter
 */
std::string utils::toml_read_string(toml::value &settings, std::string_view name, bool mandatory)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_read_string --> name is empty" << LOG_END
        return {};
    }

    std::string value;

    try {
        // read dataref
        if (toml_contains(settings, name, mandatory)) {
            value = settings[name.data()].as_string();
            LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                      << value << "'" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return value;
}


/**
 * Read the values of a string array
 */
std::vector<std::string> utils::toml_read_string_array(toml::value &settings,
                                                       std::string_view name,
                                                       bool mandatory)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_read_string --> name is empty" << LOG_END
        return {};
    }

    std::vector<std::string> list;

    try {
        // read dataref array
        if (toml_contains(settings, name, mandatory) && (settings[name.data()].is_array())) {
            for (int i = 0; i < settings[name.data()].size(); i++) {
                std::string value = settings[name.data()][i].as_string();

                LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                          << value << "'" << LOG_END

                if (!value.empty())
                    list.emplace_back(value);
            }
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return list;
}


/**
 * Read the value of an integer
 */
int utils::toml_read_int(toml::value &settings, std::string_view name, bool mandatory)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_read_int --> name is empty" << LOG_END
        return -1;
    }

    int value = -1;

    try {
        // read dataref
        if (toml_contains(settings, name, mandatory)) {
            value = static_cast<int>(settings[name.data()].as_integer());
            LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                      << value << "'" << LOG_END
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return value;
}


/**
 * Return the text of a text message type
 */
std::string utils::text_msg_type_as_text(text_msg_type type)
{
    switch (type) {
        case text_msg_type::all:
            return "All";

        case text_msg_type::error:
            return "Error";

        case text_msg_type::warn:
            return "Warning";

        case text_msg_type::info:
            return "Information";

        case text_msg_type::debug:
            return "Debug";
    }

    return {"Debug"};
}


/**
 * Return the text of a midi message type
 */
std::string utils::midi_msg_type_as_text(midi_type type)
{
    switch (type) {
        case midi_type::inbound:
            return "Inbound";

        case midi_type::outbound:
            return "Outbound";
    }

    return "Unknown";
}


/**
 * Return the text of a log level
 */
std::string utils::log_level_as_text(log_level level)
{
    switch (level) {
        case log_level::error:
            return "Errors";

        case log_level::warn:
            return "Warnings";

        case log_level::info:
            return "Information";

        case log_level::debug:
            return "Debug";

        default:
            return {"Debug"};
    }
}


/**
 * Return the code of a log level
 */
std::string utils::log_level_as_code(log_level level)
{
    switch (level) {
        case log_level::error:
            return "E";

        case log_level::warn:
            return "W";

        case log_level::info:
            return "I";

        case log_level::debug:
            return "D";

        default:
            return {"D"};
    }
}


/**
 * Return the log level for a given code
 */
log_level utils::log_level_from_code(std::string_view code)
{
    if (code == "E")
        return log_level::error;
    else if (code == "W")
        return log_level::warn;
    else if (code == "I")
        return log_level::info;
    else
        return log_level::debug;
}


/**
 * Return the outbound mode for a given integer
 */
mode_out utils::mode_out_from_int(int mode) {
    if (mode == 1)
        return mode_out::on_change;
    else
        return mode_out::permanent;
}


/**
 * Convert a time point into a string
 */
std::string utils::time_to_string(time_point time)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&t);

    // format into a string
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}


/**
 * Return MIDI channel and control change as combined string
 */
std::string utils::ch_cc(const unsigned int ch, const unsigned int cc)
{
    return std::to_string(ch) + "_" + std::to_string(cc);
}


/**
 * Create all required preference folders
 */
bool utils::create_preference_folders(const std::shared_ptr<xplane> &xp)
{
    // check preference folder
    if (!std::filesystem::exists(xp->preferences_path())) {
        LOG_INFO << "Directory '" << xp->preferences_path() << "' not found" << LOG_END

        if (std::filesystem::create_directory(xp->preferences_path())) {
            LOG_INFO << "Directory '" << xp->preferences_path() << "' created" << LOG_END
        } else {
            LOG_ERROR << "Could not create directory '" << xp->preferences_path() << "'" << LOG_END
            return false;
        }
    }

    // check profiles folder
    if (!std::filesystem::exists(xp->profiles_path())) {
        LOG_INFO << "Directory '" << xp->profiles_path() << "' not found" << LOG_END

        if (std::filesystem::create_directory(xp->profiles_path())) {
            LOG_INFO << "Directory '" << xp->profiles_path() << "' created" << LOG_END
        } else {
            LOG_ERROR << "Could not create directory '" << xp->profiles_path() << "'" << LOG_END
            return false;
        }
    }

    return true;
}

} // Namespace xmidictrl
