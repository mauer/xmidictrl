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

#include "inbound_worker.h"

// Standard
#include <mutex>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a task to be executed
 */
void inbound_worker::add_task(const std::shared_ptr<inbound_task>& in_task)
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    m_tasks.push(in_task);
}


/**
 * Process inbound tasks
 */
void inbound_worker::process()
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    std::queue<std::shared_ptr<inbound_task>> temp_list;

    // process the midi inbound queue for each midi device
    while (!m_tasks.empty()) {
        std::shared_ptr<inbound_task> task = m_tasks.front();

        //if (task == nullptr || task->map == nullptr)
        if (task == nullptr)
            continue;

        // perform the action related to the mapping
        if (!task->mapping->execute(*task->msg, task->sl_value)) {
            // store in temp list
            temp_list.push(task);
        }

        // delete entry from list
        m_tasks.pop();
    }

    // add temp tasks to queue again, will be executed in next flight loop
    while (!temp_list.empty()) {
        std::shared_ptr<inbound_task> t = temp_list.front();
        m_tasks.push(t);

        temp_list.pop();
    }
}

} // Namespace xmidictrl