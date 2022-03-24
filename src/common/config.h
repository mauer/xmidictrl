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

#ifndef CONFIG_H
#define CONFIG_H

// Standard
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "xplane.h"

namespace xmidictrl {

class config {
public:
    explicit config(xplane *xp);
    virtual ~config() = default;

    bool load(std::string_view filename);
    virtual void close();


protected:
    toml::value m_config {};

    xplane *m_xp;

private:
    std::string m_filename {};
};

} // Namespace xmidictrl

#endif // CONFIG_H
