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

#ifndef MAP_H
#define MAP_H

// Standard
#include <memory>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class map {
public:
    explicit map(std::shared_ptr<xplane> xplane);
    virtual ~map() = default;

    typedef std::shared_ptr<map> ptr;

    virtual map_type type();

    [[nodiscard]] int ch() const;
    [[nodiscard]] int cc() const;

    virtual void read_config(toml::value &data);
    virtual bool check();

protected:
    void read_ch(toml::value &data);
    void read_cc(toml::value &data);

    std::shared_ptr<xplane> m_xp;

private:
    int m_ch {-1};
    int m_cc {-1};
};

} // Namespace xmidictrl

#endif // MAP_H
