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

#ifndef XMC_INFO_WINDOW_H
#define XMC_INFO_WINDOW_H

// Standard
#include <map>

// XMidiCtrl
#include "environment.h"
#include "imgui_window.h"
#include "info_message.h"
#include "settings.h"
#include "text_logger.h"

namespace xmidictrl {

class info_window : public imgui_window {
public:
    info_window(text_logger& in_log, environment& in_env);
    ~info_window() override = default;

    void show() override;

protected:
    void create_widgets() override;

    // constants
    const int c_row_height = 25;
};

} // Namespace xmidictrl

#endif // XMC_INFO_WINDOW_H
