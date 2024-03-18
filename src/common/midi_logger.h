//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#ifndef XMC_MIDI_LOGGER_H
#define XMC_MIDI_LOGGER_H

// Standard
#include <deque>
#include <memory>

// XMidiCtrl
#include "midi_message.h"

namespace xmidictrl {

class midi_logger {
public:
    explicit midi_logger(bool in_enabled, int in_max_messages);
    ~midi_logger() = default;

    void clear();
    size_t count();

    void enable();
    void disable();

    void set_max_messages(int in_max_messages);

    midi_message* message(int in_index);

    void add(const std::shared_ptr<midi_message>& in_msg);

private:
    void adjust_log_size();

    bool m_enabled;
    int m_max_messages;

    std::deque<std::shared_ptr<midi_message>> m_messages;
};

} // Namespace xmidictrl

#endif // XMC_MIDI_LOGGER_H