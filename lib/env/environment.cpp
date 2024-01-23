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

#include "environment.h"

// Standard
#include <mutex>

// XMidiCtrl
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
environment::environment(text_logger& in_log)
	: m_log(in_log)
{
	// create the inbound worker
	m_worker = std::make_unique<inbound_worker>();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Returns the current plugin settings
 */
xmidictrl::settings& environment::settings()
{
	if (m_settings == nullptr)
		// plugin settings
		m_settings = std::make_unique<xmidictrl::settings>(log(), preferences_path());

	return *m_settings;
}


/**
 * Returns the task worker
 */
inbound_worker& environment::worker()
{
	return *m_worker;
}


/**
 * Return all info messages
 */
std::map<std::string, std::shared_ptr<info_message>>& environment::info_messages()
{
	return m_info_msg;
}


/**
 * Display an info message on the screen
 */
void environment::show_info_message(std::string_view in_id, std::string_view in_msg, int in_seconds)
{
	if (settings().info_disabled())
		return;

	std::shared_ptr<info_message> msg;

	if (in_seconds == -1)
		msg = std::make_shared<info_message>(settings().info_seconds());
	else
		msg = std::make_shared<info_message>(in_seconds);

	msg->id = in_id;
	msg->text = in_msg;

	std::mutex mutex;
	std::scoped_lock lock(mutex);

	m_info_msg.insert_or_assign(msg->id, msg);
}


/**
 * Create all required preference folders
 */
bool environment::create_preference_folders(text_logger& in_log)
{
	// check preference folder
	if (!utils::create_directory(in_log, preferences_path()))
		return false;

	// check profiles folder
	if (!utils::create_directory(in_log, profiles_path()))
		return false;

	// check includes folder
	if (!utils::create_directory(in_log, includes_path()))
		return false;

	return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the text logger
 */
text_logger& environment::log() const
{
	return m_log;
}

} // Namespace xmidictrl