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

#ifndef XMC_MAP_PARAM_OUT_H
#define XMC_MAP_PARAM_OUT_H

// Standard
#include <string>

// XMidiCtrl
#include "device_settings.h"
#include "map_param.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class map_param_out : public map_param {
public:
	map_param_out(std::string_view in_sl_value, text_logger& in_msg, outbound_send_mode in_send_mode = outbound_send_mode::permanent);

	map_param_type type() override;

	text_logger& log();
	outbound_send_mode send_mode();

private:
	text_logger& m_log;

	outbound_send_mode m_send_mode;
};

} // Namespace xmidictrl

#endif // XMC_MAP_PARAM_OUT_H
