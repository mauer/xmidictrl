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

#ifndef _MAP_IN_PNP_H_
#define _MAP_IN_PNP_H_

// Standard
#include <memory>
#include <string>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "midi_message.h"
#include "types.h"

namespace xmidictrl {

enum class CommandType {
    Push,
    Pull
};

class map_in_pnp : public map_in {
public:
    explicit map_in_pnp(xplane::ptr xp, toml::value &settings);
    ~map_in_pnp() override = default;

    typedef std::shared_ptr<map_in_pnp> ptr;

    mapping_type type() override;

    void set_command_push(std::string_view command);
    [[nodiscard]] std::string_view command_push() const;

    void set_command_pull(std::string_view command);
    [[nodiscard]] std::string_view command_pull() const;

    void set_command_type(CommandType commandType);

    bool check() override;
    void execute(midi_message::ptr midi_event) override;

protected:
    void read_config(toml::value &settings) override;

private:
    CommandType m_command_type {CommandType::Push};

    std::string m_command_push;
    std::string m_command_pull;
};

} // Namespace XMidiCtrl

#endif // _MAP_IN_PNP_H_
