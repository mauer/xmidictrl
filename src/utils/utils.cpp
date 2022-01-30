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
 * Check if the given key is an array
 */
bool utils::toml_is_array(toml::value &settings, std::string_view name)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_is_array --> name is empty" << LOG_END
        return false;
    }

    try {
        // read dataref
        if (settings.contains(name.data())) {
            if (settings[name.data()].is_array())
                return true;
            else
                return false;
        } else {
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
 * Read the values of a string array and return as set
 */
std::set<std::string> utils::toml_read_str_set_array(toml::value &settings,
                                                     std::string_view name,
                                                     bool mandatory)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_read_string --> name is empty" << LOG_END
        return {};
    }

    std::set<std::string> list;

    try {
        // read dataref array
        if (toml_contains(settings, name, mandatory) && (settings[name.data()].is_array())) {
            for (int i = 0; i < settings[name.data()].size(); i++) {
                std::string value = settings[name.data()][i].as_string();

                LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                          << value << "'" << LOG_END

                if (!value.empty())
                    list.insert(value);
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
 * Read the values of a string array and return as vector
 */
std::vector<std::string> utils::toml_read_str_vector_array(toml::value &settings,
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
                    list.push_back(value);
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
            if (settings[name.data()].is_integer()) {
                value = static_cast<float>(settings[name.data()].as_integer());

                LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                          << value << "'" << LOG_END
            } else {
                LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str()
                          << LOG_END
                LOG_ERROR << " --> Parameter '" << name << "' is not numeric" << LOG_END
            }
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return value;
}


/**
 * Read the value of a float
 */
float utils::toml_read_float(toml::value &settings, std::string_view name, bool mandatory, float fallback)
{
    if (name.empty()) {
        LOG_ERROR << "Internal error (toml_read_float --> name is empty" << LOG_END
        return fallback;
    }

    float value = fallback;

    try {
        // read dataref
        if (toml_contains(settings, name, mandatory)) {
            if (settings[name.data()].is_floating()) {
                value = static_cast<float>(settings[name.data()].as_floating());

                LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                          << value << "'" << LOG_END
            } else if (settings[name.data()].is_integer()) {
                value = static_cast<float>(settings[name.data()].as_integer());

                LOG_DEBUG << " --> Line " << settings.location().line() << " :: Parameter '" << name << "' = '"
                          << value << "'" << LOG_END

            } else {
                LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str()
                          << LOG_END
                LOG_ERROR << " --> Parameter '" << name << "' is not numeric" << LOG_END
            }
        }
    } catch (toml::type_error &error) {
        LOG_ERROR << "Line " << settings.location().line() << " :: " << settings.location().line_str() << LOG_END
        LOG_ERROR << " --> Error reading mapping" << LOG_END
        LOG_ERROR << error.what() << LOG_END
    }

    return value;
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
mode_out utils::mode_out_from_int(int mode)
{
    if (mode == 1)
        return mode_out::on_change;
    else
        return mode_out::permanent;
}


/**
 * Return the encoder mode for a given string
 */
encoder_mode utils::encoder_mode_from_code(std::string_view mode)
{
    if (mode == "range")
        return encoder_mode::range;
    else
        return encoder_mode::relative;
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
 * Create a unique key for mappings
 */
std::string utils::create_map_key(const unsigned char ch, std::string_view type_code, const unsigned char data)
{
    std::stringstream ss;
    ss << static_cast<int>(ch) << type_code << static_cast<int>(data);

    return ss.str();
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
