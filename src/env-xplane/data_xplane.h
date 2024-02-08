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

#ifndef DATA_XPLANE_H
#define DATA_XPLANE_H

// Standard
#include <map>
#include <memory>
#include <vector>
#include <string>

// X-Plane SDK
#include "XPLMDataAccess.h"

// XMidiCtrl
#include "data.h"
#include "data_item.h"
#include "text_logger.h"

namespace xmidictrl {

class data_xplane : public data {
public:
    data_xplane() = default;
    ~data_xplane() override = default;

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

private:
    data_item *retrieve_data(std::string_view in_name);

    static int get_index(text_logger &in_log, std::string_view in_name);

    static int read_int(const data_item *item);
    static float read_float(const data_item *item);
    static double read_double(const data_item *item);
    static std::string read_byte(const data_item *item);

    static float read_float_array(text_logger &in_log, const data_item *in_item, int in_index);
    static std::vector<float> read_float_array(const data_item *item);

    static int read_int_array(text_logger &in_log, const data_item *in_item, int in_index);
    static std::vector<int> read_int_array(const data_item *item);

    static void write_int(const data_item *item, int value);
    static void write_int_array(const data_item *item, int index, int value);

    static void write_float(const data_item *item, float value);
    static void write_float_array(const data_item *item, int index, float value);

    static void write_double(const data_item *item, double value);
    static void write_byte(const data_item *item, std::string_view value);

    static std::string toggle_int(text_logger &in_log,
                                  const data_item *in_item,
                                  std::string_view in_value_on,
                                  std::string_view in_value_off);
    static std::string toggle_int_array(text_logger &in_log,
                                        const data_item *in_item,
                                        int in_index,
                                        std::string_view in_value_on,
                                        std::string_view in_value_off);
    static std::string toggle_float(text_logger &in_log,
                                    const data_item *in_item,
                                    std::string_view in_value_on,
                                    std::string_view in_value_off);
    static std::string toggle_float_array(text_logger &in_log,
                                          const data_item *in_item,
                                          int in_index,
                                          std::string_view in_value_on,
                                          std::string_view in_value_off);
    static std::string toggle_double(text_logger &in_log,
                                     const data_item *in_item,
                                     std::string_view in_value_on,
                                     std::string_view in_value_off);
    static std::string toggle_byte(text_logger &in_log,
                                   const data_item *in_item,
                                   std::string_view in_value_on,
                                   std::string_view in_value_off);

    std::map<std::string, std::unique_ptr<data_item>> m_data_cache {};
};

} // Namespace xmidictrl

#endif // DATA_XPLANE_H
