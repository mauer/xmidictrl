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

#ifndef DATA_STANDALONE_H
#define DATA_STANDALONE_H

// Standard
#include <vector>
#include <string>

// XMidiCtrl
#include "data.h"
#include "data_item.h"
#include "text_logger.h"

namespace xmidictrl {

class data_standalone : public data {
public:
    data_standalone() = default;
    ~data_standalone() override = default;

    bool check(std::string_view in_name) override;

    bool read(text_logger &in_log, std::string_view in_name, std::string &out_value) override;
    bool read(text_logger &in_log, std::string_view in_name, float &out_value) override;
    bool read(text_logger &in_log, std::string_view in_name, std::vector<float> &out_values) override;
    bool read(text_logger &in_log, std::string_view in_name, std::vector<int> &out_values) override;

    bool write(text_logger &in_log, std::string_view in_name, std::string_view in_value) override;
    bool write(text_logger &in_log, std::string_view in_name, float in_value) override;

    std::string toggle(text_logger &in_log,
                       std::string_view in_name,
                       std::string_view in_value_on,
                       std::string_view in_value_off) override;

};

} // Namespace xmidictrl

#endif // DATA_STANDALONE_H
