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

#ifndef UTILS_H
#define UTILS_H

// Standard
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

class utils {
public:
    static std::string read_string_parameter(toml::value &settings, std::string_view parameter, bool mandatory = true);
    static int read_int_parameter(toml::value &settings, std::string_view parameter, bool mandatory = true);

    static std::string text_msg_type_as_text(text_msg_type type);
    static std::string midi_msg_type_as_text(midi_type type);

    static std::string log_level_as_text(log_level level);
    static std::string log_level_as_code(log_level level);
    static log_level log_level_from_code(std::string_view code);

    static std::string time_to_string(time_point time);
    static std::string ch_cc(int ch, int cc);
};

} // Namespace xmidictrl

#endif // UTILS_H
