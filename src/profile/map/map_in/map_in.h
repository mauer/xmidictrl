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

#ifndef MAP_IN_H
#define MAP_IN_H

// Standard
#include <memory>

// XMidiCtrl
#include "map.h"
#include "midi_message.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class map_in : public map {
public:
    map_in(std::shared_ptr<xplane> xp);
    ~map_in() override = default;

    typedef std::shared_ptr<map_in> ptr;

    virtual void execute(midi_message &msg) = 0;
};

} // Namespace xmiditrl

#endif // MAP_IN_H
