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
#include "ImGuiWindow.h"
#include "profile.h"
#include "xplane.h"

namespace xmidictrl {

class profile_window : public ImGuiWindow {
public:
    profile_window(std::shared_ptr<xplane> xp, std::shared_ptr<profile> prf);
    ~profile_window() override = default;

protected:
    void create_widgets() override;

private:
    std::shared_ptr<profile> m_prf;
};

} // Namespace xmidictrl

#endif // profile_WINDOW_H
