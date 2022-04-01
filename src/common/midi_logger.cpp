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
#include <vector>

// XMidiCtrl

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Set the logging state
 */
void midi_logger::set_state(bool in_state)
{
    m_state = in_state;
}


/**
 * Return the logging state
 */
bool midi_logger::state() const
{
    return m_state;
}


/**
 * Set the max message size
 */
void midi_logger::set_max_size(int in_size)
{
    if (in_size > 0)
        m_max_size = in_size;
    else
        m_max_size = 1;
}


/**
 * Return the max message size
 */
int midi_logger::max_size() const
{
    return m_max_size;
}


/**
 * Clear all messages
 */
void midi_logger::clear()
{
    m_messages.clear();
}


/**
 * Return the number of messages
 */
size_t midi_logger::count()
{
    return m_messages.size();
}


/**
 * Return a specific midi message
 */
midi_message *midi_logger::message(int in_index)
{
    return m_messages.at(in_index).get();
}


/**
 * Log a MIDI message
 */
void midi_logger::add(const std::shared_ptr<midi_message> &in_msg)
{
    if (!m_state)
        return;

    while (m_messages.size() >= m_max_size)
        m_messages.pop_front();

    m_messages.push_back(in_msg);
}

} // Namespace xmidictrl
