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

#include "device.h"

#include <utility>

// XMidiCtrl
#include "conversions.h"
#include "map_in_cmd.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
device::device(text_logger& in_text_log, environment& in_env, std::unique_ptr<device_settings> in_settings)
	: m_text_log(in_text_log)
	, m_env(in_env)
	, m_settings(std::move(in_settings))
{
	m_map_in = std::make_unique<map_in_list>();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the environment
 */
environment& device::env()
{
	return m_env;
}


/**
 * Return the device settings
 */
device_settings& device::settings()
{
	return *m_settings;
}


/**
 * Return the inbound mapping list
 */
map_in_list& device::mapping_in()
{
	return *m_map_in;
}


/**
 * Return the current sublayer dataref value
 */
std::string device::sl_value() const
{
	return m_sl_value;
}


/**
 * Set the sublayer dataref value
 */
void device::set_sl_value(std::string_view in_sl_value)
{
	m_sl_value = in_sl_value;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the text logger
 */
text_logger& device::text_log()
{
	return m_text_log;
}

} // Namespace xmidictrl