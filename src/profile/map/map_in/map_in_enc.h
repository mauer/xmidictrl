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

#ifndef MAP_IN_ENC_H
#define MAP_IN_ENC_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"

namespace xmidictrl {

class map_in_enc : public map_in {
public:
    explicit map_in_enc(std::shared_ptr<xplane> xp);
    ~map_in_enc() override = default;

    map_type type() override;

    void set_command_up(std::string_view command_up);
    [[nodiscard]] std::string_view command_up() const;

    void set_command_fast_up(std::string_view command_fast_up);
    [[nodiscard]] std::string_view command_fast_up() const;

    void set_command_down(std::string_view command_down);
    [[nodiscard]] std::string_view command_down() const;

    void set_command_fast_down(std::string_view command_fast_down);
    [[nodiscard]] std::string_view command_fast_down() const;

    void read_config(toml::value &settings) override;
    bool check() override;

    void execute(midi_message &midi_event) override;

private:
    std::string m_command_up {};
    std::string m_command_down {};

    std::string m_command_fast_up {};
    std::string m_command_fast_down {};
};

} // Namespace xmidictrl

#endif // MAP_IN_ENC_H
