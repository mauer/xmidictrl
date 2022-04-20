//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
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

#ifndef MESSAGES_WINDOW_H
#define MESSAGES_WINDOW_H

// Standard
#include <memory>

// XMidiCtrl
#include "imgui_window.h"
#include "midi_logger.h"
#include "settings.h"
#include "text_logger.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class messages_window : public imgui_window {
public:
    messages_window(text_logger &in_text_log, midi_logger &in_midi_log, xplane &in_xp, settings &in_settings);
    ~messages_window() override;

protected:
    void create_widgets() override;

private:
    void create_tab_text_msg();
    void create_tab_midi_msg();

    void add_text_row(text_log_msg *in_msg);
    void add_midi_row(midi_message *in_msg);

    static void draw_icon(const char *in_icon, std::string_view in_text);

    sort_mode m_text_sort_mode {sort_mode::ascending};
    sort_mode m_midi_sort_mode {sort_mode::ascending};

    ImGuiTableColumnFlags m_text_msg_flags;
    ImGuiTableColumnFlags m_midi_msg_flags;

    settings &m_settings;
    midi_logger &m_midi_log;
};

} // Namespace xmidictrl

#endif // MESSAGES_WINDOW_H
