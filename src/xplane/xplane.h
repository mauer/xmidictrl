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

#ifndef _XPLANE_H_
#define _XPLANE_H_

// Standard
#include <memory>
#include <string>

// XMidiCtrl
#include "commands.h"
#include "data.h"

namespace xmidictrl {

class xplane {
public:
    xplane();
    ~xplane() = default;

    typedef std::shared_ptr<xplane> ptr;

    XPLMPluginID plugin_id();

    std::string_view plugin_path();
    std::string_view xplane_path();
    std::string_view preferences_path();
    std::string_view profiles_path();

    static std::string current_aircraft_path();
    std::string current_aircraft_author();
    std::string current_aircraft_icao();
    std::string current_aircraft_descr();

    commands &cmd();
    data &datarefs();

private:
    XPLMPluginID m_plugin_id {-1};

    std::string m_xplane_path {};
    std::string m_plugin_path {};
    std::string m_preferences_path {};
    std::string m_profiles_path {};

    std::unique_ptr<commands> m_commands;
    std::unique_ptr<data> m_datarefs;
};

} // namespace xmidictrl

#endif // _XPLANE_H_