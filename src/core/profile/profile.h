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
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "config.h"
#include "device_list.h"
#include "mapping.h"
#include "text_logger.h"
#include "xmc_types.h"
#include "app_services.h"

namespace xmidictrl {

enum class filename_prefix {
    none,
    icao,
    acf_name
};

class profile : public config {
public:
    explicit profile(text_logger &in_text_log, midi_logger &in_midi_log, app_services& in_app, app_settings &in_settings);
    ~profile() override;

    bool load();
    void close();

    [[nodiscard]] bool loaded() const;

    [[nodiscard]] std::string_view filename() const;

    text_logger &log();
    [[nodiscard]] bool has_errors() const;

    std::string title();
    std::string version();

    std::string get_filename_aircraft_path(filename_prefix in_prefix);
    std::string get_filename_profiles_path(filename_prefix in_prefix);

    [[nodiscard]] std::string_view sl_dataref() const;

    void process(text_logger &in_log);

private:
    void clear();

    std::string find_profile();

    void create_device_list();

    void create_init_mapping(int in_dev_no, toml::array in_settings, const std::shared_ptr<device> &out_device);
    void create_inbound_mapping(int in_dev_no, toml::array in_settings, const std::shared_ptr<device> &out_device);
    void create_outbound_mapping(int in_dev_no, toml::array in_settings, const std::shared_ptr<device> &out_device);

    static map_type translate_map_type(std::string_view in_type_str);
    map_type read_mapping_type(toml::value &in_settings);

    app_settings &m_settings;

    bool m_loaded {false};
    bool m_init_send {false};

    text_logger &m_plugin_log;
    midi_logger &m_midi_log;

    std::unique_ptr<text_logger> m_profile_log;
    std::unique_ptr<device_list> m_device_list;

    std::string m_sl_dataref {};
    std::string m_filename {};
};

} // Namespace xmidictrl
