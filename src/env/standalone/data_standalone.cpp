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

#include "data_standalone.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check if a dataref is valid
 */
bool data_standalone::check(std::string_view in_name)
{
    return true;
}


/**
 * Read a dataref
 */
bool data_standalone::read(text_logger &in_log, std::string_view in_name, std::string &out_value)
{
    out_value = std::string();

    return true;
}


/**
 * Read a numeric dataref
 */
bool data_standalone::read(text_logger &in_log, std::string_view in_name, float &out_value)
{
    out_value = 0.0f;

    return true;
}


/**
 * Read a dataref as float array
 */
bool data_standalone::read(text_logger &in_log, std::string_view in_name, std::vector<float> &out_values)
{
    out_values = {};

    return true;
}


/**
 * Read a dataref as integer array
 */
bool data_standalone::read(text_logger &in_log, std::string_view in_name, std::vector<int> &out_values)
{
    out_values = {};

    return true;
}


/**
 * Write an value to a dataref
 */
bool data_standalone::write(text_logger &in_log, std::string_view in_name, std::string_view in_value)
{
    return true;
}


/**
 * Write an value to a numeric dataref
 */
bool data_standalone::write(text_logger &in_log, std::string_view in_name, float in_value)
{
    return true;
}


/**
 * Toggle a dataref between on and off
 */
std::string data_standalone::toggle(text_logger &in_log,
                                    std::string_view in_name,
                                    std::string_view in_value_on,
                                    std::string_view in_value_off)
{
    return {};
}

} // Namespace xmidictrl