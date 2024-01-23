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

#include "command_tests.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Begin a X-Plane command
 */
void command_tests::begin(text_logger& in_log, std::string_view in_cmd)
{
	m_current_command = in_cmd;
}


/**
 * End a X-Plane command
 */
void command_tests::end(text_logger& in_log, std::string_view in_cmd)
{
	m_current_command.clear();
	m_last_command = in_cmd;
}


/**
 * Execute a X-Plane command
 */
void command_tests::execute(text_logger& in_log, std::string_view in_cmd)
{}

/**
 * Return the current command
 */
std::string command_tests::current_command() const
{
	return m_current_command;
}


/**
 * Return the last executed command
 */
std::string command_tests::last_command() const
{
	return m_last_command;
}

} // Namespace xmidictrl