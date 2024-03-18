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

#ifndef XMC_LOG_VIEWER_H
#define XMC_LOG_VIEWER_H

// Standard
#include <memory>

// XMidiCtrl
#include "environment.h"
#include "imgui_window.h"
#include "midi_logger.h"
#include "settings.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class log_viewer : public imgui_window {
public:
	log_viewer(text_logger& in_text_log, environment& in_env);
	~log_viewer() override = default;

protected:
	void create_widgets() override;

private:
	// constants
	static constexpr unsigned int c_no_msg_page {500};

	// members
	sort_mode m_log_sort_mode {sort_mode::ascending};
	unsigned int m_page {0};

	ImGuiTableColumnFlags m_log_msg_flags;

	void add_log_row(size_t in_no, text_log_msg* in_msg);
};

} // Namespace xmidictrl

#endif // XMC_LOG_VIEWER_H
