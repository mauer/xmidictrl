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

#ifndef XMC_MAP_IN_H
#define XMC_MAP_IN_H

// Standard
#include <memory>

// XMidiCtrl
#include "environment.h"
#include "label.h"
#include "map.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

// Inbound mapping types
enum class map_in_type {
    none,
    command,
    dataref,
    encoder,
    push_pull,
    slider
};




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map_in : public map {
public:
    explicit map_in(environment& in_env);
    ~map_in() override = default;

    virtual map_in_type type();

    virtual void read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config);

    virtual std::string map_text_label() = 0;
    virtual std::string map_text_cmd_drf() = 0;
    virtual std::string map_text_parameter() = 0;

protected:
    environment& env();

    virtual std::string toggle_dataref(text_logger& in_log, std::string_view in_dataref, std::vector<std::string>& in_values);

private:
    // members
    environment& m_env;
};

} // Namespace xmiditrl

#endif // XMC_MAP_IN_H
