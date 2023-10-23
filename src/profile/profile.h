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

#ifndef XMC_PROFILE_H
#define XMC_PROFILE_H

// Standard
#include <memory>
#include <string>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "device_list.h"
#include "environment.h"
#include "map.h"
#include "text_logger.h"
#include "settings.h"
#include "types.h"

namespace xmidictrl {

enum class filename_prefix {
    none,
    icao,
    acf_name
};

class profile {
public:
    explicit profile(text_logger& in_text_log, midi_logger& in_midi_log, environment& in_env);
    ~profile();

    bool load();
    void close();

    [[nodiscard]] bool loaded() const;

    [[nodiscard]] std::string_view filename() const;

    text_logger& log();
    [[nodiscard]] bool has_errors() const;

    std::string title();
    std::string version();

    device_list& devices();

    std::string get_filename_aircraft_path(filename_prefix in_prefix);
    std::string get_filename_profiles_path(filename_prefix in_prefix);

    [[nodiscard]] std::string_view sl_dataref() const;

    void process(text_logger& in_log);

private:
    void clear();

    std::string find_profile();

    void create_device_list();
    void create_virtual_device();

    void create_device(const toml::value& in_params, bool in_is_virtual, size_t in_dev_no = 0);
    std::shared_ptr<device_settings> create_device_settings(toml::value in_params,
                                                            bool in_is_virtual,
                                                            size_t in_dev_no = 0);

    void add_mappings_from_include(const std::shared_ptr<device>& in_device);
    void create_device_mappings(toml::value in_params, const std::shared_ptr<device>& in_device);

    void create_init_mapping(toml::array in_params, const std::shared_ptr<midi_device>& in_device);
    void create_inbound_mapping(toml::array in_params, const std::shared_ptr<device>& in_device);
    void create_outbound_mapping(toml::array in_params, const std::shared_ptr<midi_device>& in_device);

    static map_type translate_map_type(std::string_view in_type_str);
    map_type read_mapping_type(toml::value& in_params);

    static std::string get_log_prefix_from_device(const std::shared_ptr<device>& in_device);
    static std::string get_log_prefix(bool in_is_virtual, size_t in_dev_no = 0);

    toml::value m_config {};

    environment& m_env;

    bool m_loaded {false};
    bool m_init_send {false};

    text_logger& m_plugin_log;
    midi_logger& m_midi_log;

    std::unique_ptr<text_logger> m_profile_log;
    std::unique_ptr<device_list> m_device_list;

    std::string m_sl_dataref {};
    std::string m_filename {};
};

} // Namespace xmidictrl

#endif // XMC_PROFILE_H