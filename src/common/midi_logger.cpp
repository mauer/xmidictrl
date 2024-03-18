//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
midi_logger::midi_logger(bool in_enabled)
	: m_enabled(in_enabled)
{}




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
 * Enable midi logging
 */
void midi_logger::enable()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_enabled = true;
}


/**
 * Disable midi logging
 */
void midi_logger::disable()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_enabled = false;
}


/**
 * Return a specific midi message
 */
midi_message* midi_logger::message(size_t in_index)
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	return m_messages.at(in_index).get();
}


/**
 * Log a MIDI message
 */
void midi_logger::add(const std::shared_ptr<midi_message>& in_msg)
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	if (!m_enabled)
		return;

	m_messages.push_back(in_msg);
}

} // Namespace xmidictrl
