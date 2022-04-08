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

#ifndef PROFILE_WINDOW_H
#define PROFILE_WINDOW_H

// XMidiCtrl
#include "imgui_window.h"
#include "profile.h"
#include "xplane.h"

namespace xmidictrl {

class profile_window : public imgui_window {
public:
    profile_window(text_logger &in_log, xplane &in_xp, profile &in_profile);
    ~profile_window() override = default;

protected:
    void create_widgets() override;

private:
    void create_tab_general();
    void create_tab_errors_warnings();

    profile &m_profile;
};

} // Namespace xmidictrl

#endif // profile_WINDOW_H
