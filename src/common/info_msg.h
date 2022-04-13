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

#ifndef INFO_MSG_H
#define INFO_MSG_H

// Standard
#include <string>

// XMidiCtrl
#include "types.h"

namespace xmidictrl {

struct info_msg {
    time_point exp_time;
    std::string id;
    std::string text;

    explicit info_msg(int in_seconds)
    {
        exp_time = std::chrono::system_clock::now();
        exp_time += std::chrono::seconds(in_seconds);
    };
};

} // Namespace xmidictrl

#endif // INFO_MSG_H