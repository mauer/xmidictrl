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

#ifndef XMC_PROFILE_WINDOW_H
#define XMC_PROFILE_WINDOW_H

// XMidiCtrl
#include "environment.h"
#include "imgui_window.h"
#include "profile.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

enum class map_page {
    init,
    inbound,
    outbound
};




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class profile_window : public imgui_window {
public:
    profile_window(text_logger& in_log, environment& in_env, profile& in_profile);
    ~profile_window() override = default;

protected:
    void create_widgets() override;

private:
    void create_tab_general();
    void create_tab_devices();
    void create_tab_device(device& in_device);
    void create_tab_errors_warnings();

    void create_title(std::string_view in_title, bool in_no_newline = true);

    void create_table_mapping_init(device& in_device);
    void create_table_mapping_in(device& in_device);
    void create_table_mapping_out(device& in_device);

    map_page m_current_map_page {map_page::inbound};

    profile& m_profile;
};

} // Namespace xmidictrl

#endif // XMC_PROFILE_WINDOW_H
