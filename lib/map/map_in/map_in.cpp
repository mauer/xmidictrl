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

#include "map_in.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in::map_in(environment& in_env)
    : map(),
      m_env(in_env)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in::type()
{
    return map_in_type::none;
}


/**
 * Read the config
 */
void map_in::read_config(text_logger& in_log, toml::value& in_data, toml::value&)
{
    // read the common data
    read_common_config(in_log, in_data);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the environment
 */
environment& map_in::env()
{
    return m_env;
}


/**
 * Toggle dataref between values
 */
std::string map_in::toggle_dataref(text_logger& in_log, std::string_view in_dataref, std::vector<std::string>& in_values)
{
    std::string new_value {};

    if (in_values.size() == 2) {
        new_value = env().drf().toggle(in_log, in_dataref, in_values[0], in_values[1]);
    } else {
        // get current value
        std::string value;
        env().drf().read(in_log, in_dataref, value);

        // search for the value in the values list
        auto it = std::find(in_values.begin(), in_values.end(), value);

        if (it != in_values.end()) {
            auto idx = std::distance(in_values.begin(), it);

            if (idx < static_cast<int>(in_values.size()) - 1)
                idx++;
            else
                idx = 0;

            new_value = in_values[idx];
        } else {
            // value not found, let's take the first one of the list
            new_value = in_values[0];
        }

        in_log.debug(fmt::format(" --> Change dataref '{}' to value '{}'", in_dataref, new_value));

        env().drf().write(in_log, in_dataref, new_value);
    }

    return new_value;
}

} // Namespace xmidictrl