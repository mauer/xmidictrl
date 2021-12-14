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

#ifndef MAP_OUT_DRF_H
#define MAP_OUT_DRF_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_out.h"
#include "midi_message.h"

namespace xmidictrl {

class map_out_drf : public map_out {
public:
    explicit map_out_drf(std::shared_ptr<xplane> xp);
    ~map_out_drf() override = default;

    map_type type() override;

    void set_dataref(std::string_view dataref);
    [[nodiscard]] std::string_view dataref() const;

    void set_value_on(std::string_view value_on);
    [[nodiscard]] std::string_view value_on() const;

    void set_value_off(std::string_view value_off);
    [[nodiscard]] std::string_view value_off() const;

    void read_config(toml::value &settings) override;
    bool check() override;

    std::shared_ptr<midi_message> execute() override;

private:
    std::string m_dataref;
    std::string m_value_on;
    std::string m_value_off;

    std::string m_value_previous;
};

} // Namespace xmidictrl

#endif // MAP_OUT_DRF_H