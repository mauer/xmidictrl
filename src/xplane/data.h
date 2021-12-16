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

#ifndef _DATA_H_
#define _DATA_H_

// Standard
#include <map>
#include <memory>
#include <vector>

// X-Plane SDK
#include "XPLMDataAccess.h"

// XMidiCtrl
#include "data_item.h"

namespace xmidictrl {

class data {
public:
    data() = default;
    ~data();

    bool read(std::string_view name, std::string &value);
    bool read(std::string_view name, std::vector<float> &values);
    bool read(std::string_view name, std::vector<int> &values);

    bool toggle(std::string_view name, std::string_view value_on, std::string_view value_off);

private:
    data_item *retrieve_data(std::string_view name);

    static int read_int(const data_item *item);
    static float read_float(const data_item *item);
    static double read_double(const data_item *item);
    static std::string read_byte(const data_item *item);

    static std::vector<float> read_float_array(const data_item *item);
    static std::vector<int> read_int_array(const data_item *item);

    static void toggle_int(const data_item *item, std::string_view value_on, std::string_view value_off);
    static void toggle_float(const data_item *item, std::string_view value_on, std::string_view value_off);
    static void toggle_double(const data_item *item, std::string_view value_on, std::string_view value_off);
    static void toggle_byte(const data_item *item, std::string_view value_on, std::string_view value_off);

    std::map<std::string, std::unique_ptr<data_item>> m_data_cache;
};

} // Namespace xmidictrl

#endif // _DATA_H_