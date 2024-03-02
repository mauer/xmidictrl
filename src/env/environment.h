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

#ifndef XMC_ENV_H
#define XMC_ENV_H

// Standard
#include <filesystem>

// XMidiCtrl
#include "commands.h"
#include "data.h"
#include "inbound_worker.h"
#include "info_message.h"
#include "settings.h"
#include "text_logger.h"

namespace xmidictrl {

class environment {
public:
    explicit environment(text_logger& in_log);

    xmidictrl::settings& settings();
    inbound_worker& worker();

    std::map<std::string, std::shared_ptr<info_message>>& info_messages();
    void show_info_message(std::string_view in_id, std::string_view in_msg, int in_seconds = -1);

    bool create_preference_folders(text_logger& in_log);

    virtual std::filesystem::path xplane_path() = 0;
    virtual std::filesystem::path plugin_path() = 0;
    virtual std::filesystem::path preferences_path() = 0;
    virtual std::filesystem::path profiles_path() = 0;
    virtual std::filesystem::path includes_path() = 0;

    virtual std::filesystem::path current_aircraft_path() = 0;

    virtual std::string current_aircraft_author() = 0;
    virtual std::string current_aircraft_icao() = 0;
    virtual std::string current_aircraft_acf_name() = 0;
    virtual std::string current_aircraft_descr() = 0;

    virtual commands& cmd() = 0;
    virtual data& drf() = 0;

protected:
    [[nodiscard]] text_logger& log() const;

private:
    text_logger& m_log;

    std::unique_ptr<xmidictrl::settings> m_settings;
    std::unique_ptr<inbound_worker> m_worker;

    std::map<std::string, std::shared_ptr<info_message>> m_info_msg;
};

} // Namespace xmidictrl

#endif // XMC_ENV_H