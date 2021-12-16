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

#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

// XMidiCtrl
#include "ImGuiWindow.h"
#include "settings.h"
#include "xplane.h"

namespace xmidictrl {

class settings_window : public ImGuiWindow {
public:
    settings_window(const std::shared_ptr<xplane>& xp, std::shared_ptr<settings> set);
    ~settings_window() override = default;

protected:
    void create_widgets() override;

private:
    std::shared_ptr<settings> m_settings;

    log_level m_log_level;
    bool m_log_midi;
    bool m_show_messages;

    std::string m_path_xplane;
    std::string m_path_plugin;
    std::string m_path_preferences;
    std::string m_path_profiles;
};

} // Namespace xmidictrl

#endif // SETTINGS_WINDOW_H