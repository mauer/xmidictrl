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

#include "midi_logger.h"

// Standard
#include <mutex>
#include <vector>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
midi_logger::midi_logger(settings& in_settings)
    : m_settings(in_settings)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Clear all messages
 */
void midi_logger::clear()
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    m_messages.clear();
}


/**
 * Return the number of messages
 */
size_t midi_logger::count()
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    return m_messages.size();
}


/**
 * Return a specific midi message
 */
midi_message* midi_logger::message(int in_index)
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    return m_messages.at(in_index).get();
}


/**
 * Log a MIDI message
 */
void midi_logger::add(const std::shared_ptr<midi_message>& in_msg)
{
    if (!m_settings.log_midi())
        return;

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    while (m_messages.size() >= m_settings.max_midi_messages())
        m_messages.pop_front();

    m_messages.push_back(in_msg);
}

} // Namespace xmidictrl
