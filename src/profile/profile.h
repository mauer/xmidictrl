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

#ifndef PROFILE_H
#define PROFILE_H

// Standard
#include <memory>
#include <string>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "config.h"
#include "device_list.h"
#include "map.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class profile : public config {
public:
    explicit profile(xplane::ptr xplane);
    ~profile() override;

    bool load();
    void close() override;

    [[nodiscard]] std::string_view filename() const;
    [[nodiscard]] bool has_errors() const;

    std::string title();
    std::string version();

    std::string get_filename_aircraft_path();
    std::string get_filename_profiles_path(bool icao, bool author);

    void process();

private:
    void clear();

    std::string get_profile_filename();

    void create_device_list();

    void create_inbound_mapping(int dev_no, toml::array settings, const std::shared_ptr<device> &device);
    void create_outbound_mapping(int dev_no, toml::array settings, const std::shared_ptr<device> &device);

    static map_type translate_map_type(std::string_view type_str);
    map_type read_mapping_type(toml::value &settings);

    std::shared_ptr<device_list> m_device_list;

    std::string m_sl_dataref {};

    std::string m_filename {};
    bool m_errors_found {false};
};

} // Namespace xmidictrl

#endif // PROFILE_H