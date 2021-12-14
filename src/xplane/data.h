//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
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