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

#ifndef INBOUND_WORKER_H
#define INBOUND_WORKER_H

// Standard
#include <memory>
#include <queue>

// XMidiCtrl
#include "inbound_task.h"
#include "text_logger.h"

namespace xmidictrl {

class inbound_worker {
public:
    explicit inbound_worker(text_logger &in_log);
    ~inbound_worker() = default;

    void add_task(const std::shared_ptr<inbound_task> &in_task);

    void process(std::string_view in_sl_value);

private:
    text_logger &m_log;

    std::queue<std::shared_ptr<inbound_task>> m_tasks {};
};

} // Namespace xmidictrl

#endif // INBOUND_WORKER_H