//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#ifndef XMC_LOG_WINDOW_H
#define XMC_LOG_WINDOW_H

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

class log_window : public imgui_window {
public:
    log_window(text_logger& in_text_log, environment& in_env);
    ~log_window() = default;

protected:
    void create_widgets() override;

private:
    void add_log_row(text_log_msg* in_msg);

    sort_mode m_log_sort_mode {sort_mode::ascending};

    ImGuiTableColumnFlags m_log_msg_flags;
};

} // Namespace xmidictrl

#endif // XMC_LOG_WINDOW_H
