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

#ifndef _MAP_IN_DRF_H_
#define _MAP_IN_DRF_H_

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"

namespace xmidictrl {

class map_in_drf : public map_in {
public:
    explicit map_in_drf(xplane::ptr xp, toml::value &settings);
    ~map_in_drf() override = default;

    mapping_type type() override;

    void set_dataref(std::string_view dataref);
    [[nodiscard]] std::string_view dataref() const;

    void set_value_on(std::string_view value_on);
    [[nodiscard]] std::string_view value_on() const;

    void set_value_off(std::string_view value_off);
    [[nodiscard]] std::string_view value_off() const;

    bool check() override;
    void execute(midi_message::ptr midi_event) override;

protected:
    virtual void read_config(toml::value &settings) override;

private:
    std::string m_dataref;
    std::string m_value_on;
    std::string m_value_off;
};

} // Namespace XMidiCtrl

#endif // _MAP_IN_DRF_H_