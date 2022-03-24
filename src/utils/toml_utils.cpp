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

#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR & DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
toml_utils::toml_utils(message_handler *messages)
    : m_messages(messages)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check if the given key is in the config
 */
bool toml_utils::contains(toml::value &settings, std::string_view name, bool mandatory)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_contains --> name is empty");
        return false;
    }

    try {
        // read dataref
        if (settings.contains(name.data())) {
            return true;
        } else {
            if (mandatory) {
                m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
                m_messages->error(" --> Parameter '%s' not found", name);
            }

            return false;
        }
    } catch (toml::type_error &error) {
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return false;
}


/**
 * Check if the given key is an array
 */
bool toml_utils::is_array(toml::value &settings, std::string_view name)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_is_array --> name is empty");
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
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return false;
}


/**
 * Read the value of a string parameter
 */
std::string toml_utils::read_string(toml::value &settings, std::string_view name, bool mandatory)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_read_string --> name is empty");
        return {};
    }

    std::string value;

    try {
        // read dataref
        if (contains(settings, name, mandatory)) {
            value = settings[name.data()].as_string();
            m_messages->debug(" --> Line %i :: Parameter '%s' = '%s'",
                              settings.location().line(),
                              name.data(),
                              value.c_str());
        }
    } catch (toml::type_error &error) {
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return value;
}


/**
 * Read the values of a string array and return as set
 */
std::set<std::string> toml_utils::read_str_set_array(toml::value &settings,
                                                     std::string_view name,
                                                     bool mandatory)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_read_string --> name is empty");
        return {};
    }

    std::set<std::string> list;

    try {
        // read dataref array
        if (contains(settings, name, mandatory) && (settings[name.data()].is_array())) {
            for (int i = 0; i < settings[name.data()].size(); i++) {
                std::string value = settings[name.data()][i].as_string();

                m_messages->debug(" --> Line %i :: Parameter '%s' = '%s'",
                                  settings.location().line(),
                                  name,
                                  value.c_str());

                if (!value.empty())
                    list.insert(value.data());
            }
        }
    } catch (toml::type_error &error) {
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return list;
}


/**
 * Read the values of a string array and return as vector
 */
std::vector<std::string> toml_utils::read_str_vector_array(toml::value &settings,
                                                           std::string_view name,
                                                           bool mandatory)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_read_string --> name is empty");
        return {};
    }

    std::vector<std::string> list;

    try {
        // read dataref array
        if (contains(settings, name, mandatory) && (settings[name.data()].is_array())) {
            for (int i = 0; i < settings[name.data()].size(); i++) {
                std::string value = settings[name.data()][i].as_string();

                m_messages->debug(" --> Line %i :: Parameter '%s' = '%s'",
                                  settings.location().line(),
                                  name,
                                  value.c_str());

                if (!value.empty())
                    list.push_back(value);
            }
        }
    } catch (toml::type_error &error) {
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return list;
}


/**
 * Read the value of an integer
 */
int toml_utils::read_int(toml::value &settings, std::string_view name, bool mandatory)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_read_int --> name is empty");
        return -1;
    }

    int value = -1;

    try {
        // read dataref
        if (contains(settings, name, mandatory)) {
            if (settings[name.data()].is_integer()) {
                value = static_cast<float>(settings[name.data()].as_integer());

                m_messages->debug(" --> Line %i :: Parameter '%s' = '%i'",
                                  settings.location().line(),
                                  name.data(),
                                  value);
            } else {
                m_messages->error("Line %i :: Parameter '%s' = '%s'",
                                  settings.location().line(),
                                  name.data(),
                                  settings.location().line_str().c_str());
                m_messages->error(" --> Parameter '%s' is not numeric", name);
            }
        }
    } catch (toml::type_error &error) {
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return value;
}


/**
 * Read the value of a float
 */
float toml_utils::read_float(toml::value &settings, std::string_view name, bool mandatory, float fallback)
{
    if (name.empty()) {
        m_messages->error("Internal error (toml_read_float --> name is empty");
        return fallback;
    }

    float value = fallback;

    try {
        // read dataref
        if (contains(settings, name, mandatory)) {
            if (settings[name.data()].is_floating()) {
                value = static_cast<float>(settings[name.data()].as_floating());

                m_messages->debug(" --> Line %i :: Parameter '%s' = '%f'", settings.location().line(), name, value);
            } else if (settings[name.data()].is_integer()) {
                value = static_cast<float>(settings[name.data()].as_integer());

                m_messages->debug(" --> Line %i :: Parameter '%s' = '%i'", settings.location().line(), name, value);
            } else {
                m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
                m_messages->error(" --> Parameter '%s' is not numeric", name);
            }
        }
    } catch (toml::type_error &error) {
        m_messages->error("Line %i :: %s", settings.location().line(), settings.location().line_str().c_str());
        m_messages->error(" --> Error reading mapping");
        m_messages->error(error.what());
    }

    return value;
}

} // Namespace xmidictrl
