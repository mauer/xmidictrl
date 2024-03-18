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

#include "hid_logger.h"

// Standard
#include <mutex>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
hid_logger::hid_logger(bool in_enabled, int in_max_messages)
	: m_enabled(in_enabled)
	, m_max_messages(in_max_messages)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Clear all messages
 */
void hid_logger::clear()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_events.clear();
}


/**
 * Return the number of messages
 */
size_t hid_logger::count()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	return m_events.size();
}


/**
 * Enable hid logging
 */
void hid_logger::enable()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_enabled = true;
}


/**
 * Disable hid logging
 */
void hid_logger::disable()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_enabled = false;
}


/**
 * Set the maximum number of hid events in the logger
 *
 * @param in_max_events max. number of events
 */
void hid_logger::set_max_events(int in_max_events)
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_max_events = in_max_events;

	adjust_log_size();
}


/**
 * Return a specific hid event
 */
hid_event* hid_logger::event(int in_index)
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	return m_events.at(in_index).get();
}


/**
 * Log a hid ev
 */
void hid_logger::add(const std::shared_ptr<hid_event>& in_evt)
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	if (!m_enabled)
		return;

	adjust_log_size();

	m_events.push_back(in_evt);
}


/**
 * Adjust the log size depending on the current settings
 */
void hid_logger::adjust_log_size()
{
	std::mutex mutex;
	std::scoped_lock lock(mutex);

	while (m_events.size() >= m_max_events)
		m_events.pop_front();
}

} // Namespace xmidictrl
