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

#include "map_param_out.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_param_out::map_param_out(std::string_view in_sl_value, text_logger& in_log, outbound_send_mode in_send_mode)
	: map_param(in_sl_value)
	, m_log(in_log)
	, m_send_mode(in_send_mode)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the parameter type (inbound/outbound)
 * @return  Parameter type
 */
map_param_type map_param_out::type()
{
	return map_param_type::out;
}


/**
 * Return the logger for text messages
 * @return Logger for text messages
 */
text_logger& map_param_out::log()
{
	return m_log;
}


/**
 * Return the send mode for the outbound message
 * @return Send mode (permanent/on_change) for the outbound message
 */
outbound_send_mode map_param_out::send_mode() const
{
	return m_send_mode;
}

} // Namespace xmidictrl
