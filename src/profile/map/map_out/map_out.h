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

// XMidiCtrl
#include "mapping.h"
#include "outbound_task.h"
#include "xmc_types.h"
#include "app_services.h"

namespace xmidictrl {

class map_out: public mapping {
public:
    explicit map_out(app_services& in_app);
    ~map_out() override = default;

    virtual void read_config(text_logger &in_log, toml::value &in_data) = 0;

    virtual std::shared_ptr<outbound_task> execute(text_logger &in_log, mode_out mode) = 0;
    virtual std::shared_ptr<outbound_task> reset() = 0;
};

} // Namespace xmidictrl
