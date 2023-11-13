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
// TODO: Name to MIDI Watcher
#ifndef XMC_MESSAGES_WINDOW_H
#define XMC_MESSAGES_WINDOW_H

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

class messages_window : public imgui_window {
public:
    messages_window(text_logger& in_text_log, midi_logger& in_midi_log, environment& in_env);
    ~messages_window() = default;

protected:
    void create_widgets() override;

private:
    void add_midi_row(midi_message* in_msg);

    static void draw_icon(const char* in_icon, std::string_view in_text = {});

    sort_mode m_midi_sort_mode {sort_mode::ascending};

    ImGuiTableColumnFlags m_midi_msg_flags;

    midi_logger& m_midi_log;
};

} // Namespace xmidictrl

#endif // XMC_MESSAGES_WINDOW_H
