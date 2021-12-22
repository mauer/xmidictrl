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

    [[nodiscard]] std::string_view sl() const;

    virtual void read_config(toml::value &data);
    virtual bool check();

protected:
    void read_ch(toml::value &data);
    void read_cc(toml::value &data);
    void read_sl(toml::value &data);

    bool check_sublayer(std::string_view sl_value);

    std::shared_ptr<xplane> m_xp;

private:
    int m_ch {-1};
    int m_cc {-1};

    std::string m_sl {};
};

} // Namespace xmidictrl

#endif // MAP_H
