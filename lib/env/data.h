//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#ifndef DATA_H
#define DATA_H

// Standard
#include <string>
#include <vector>

// XMidiCtrl
#include "text_logger.h"

namespace xmidictrl {

class data {
public:
    data() = default;
    virtual ~data() = default;

    virtual bool check(std::string_view in_name) = 0;

    virtual bool read(text_logger& in_log, std::string_view in_name, std::string& out_value) = 0;
    virtual bool read(text_logger& in_log, std::string_view in_name, float& out_value) = 0;
    virtual bool read(text_logger& in_log, std::string_view in_name, std::vector<float>& out_values) = 0;
    virtual bool read(text_logger& in_log, std::string_view in_name, std::vector<int>& out_values) = 0;

    virtual bool write(text_logger& in_log, std::string_view in_name, std::string_view in_value) = 0;
    virtual bool write(text_logger& in_log, std::string_view in_name, float in_value) = 0;

    virtual std::string toggle(text_logger& in_log,
                               std::string_view in_name,
                               std::string_view in_value_on,
                               std::string_view in_value_off) = 0;
};

} // Namespace xmidictrl

#endif // DATA_H
