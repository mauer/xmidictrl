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

#pragma once

// Standard
#include <memory>
#include <string>

// XMidiCtrl
#include "app_services.h"
#include "commands.h"
#include "data.h"
#include "text_logger.h"

namespace xmidictrl {

class xplane : public app_services {
public:
    explicit xplane(text_logger &in_log);
    ~xplane() = default;

    XPLMPluginID plugin_id();

    // app_services implementation
    std::string plugin_path();
    std::string xplane_path();
    std::string preferences_path() override;
    std::string profiles_path() override;

    static std::string current_aircraft_path();
    std::string current_aircraft_author();
    std::string current_aircraft_icao() override;
    std::string current_aircraft_acf_name() override;
    std::string current_aircraft_descr();

    simcmd_interface &cmd() override;
    simvar_access &datarefs() override;

private:
    text_logger &m_log;

    XPLMPluginID m_plugin_id {-1};

    std::string m_xplane_path {};
    std::string m_plugin_path {};
    std::string m_preferences_path {};
    std::string m_profiles_path {};

    std::unique_ptr<commands> m_commands;
    std::unique_ptr<data> m_datarefs;
};

} // namespace xmidictrl
