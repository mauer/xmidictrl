//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

#ifndef XMC_MAP_IN_LABEL_H
#define XMC_MAP_IN_LABEL_H

// Standard
#include <memory>

// XMidiCtrl
#include "label.h"
#include "map_in.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in_label : public map_in {
public:
    explicit map_in_label(environment& in_env);
    ~map_in_label() override = default;

    void read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config) override;

    std::string map_text_label() override;

protected:
    std::string toggle_dataref(text_logger& in_log, std::string_view in_dataref, std::vector<std::string>& in_values) override;

    void display_label(text_logger& in_log, float in_value);
    void display_label(text_logger& in_log, std::string_view in_value);

private:
    void read_label(text_logger& in_log, toml::value& in_data, toml::value& in_config);

    // constants
    static constexpr std::string_view c_cfg_label {"label"};

    // members
    std::unique_ptr<label> m_label {nullptr};
};

} // Namespace xmiditrl

#endif // XMC_MAP_IN_LABEL_H
