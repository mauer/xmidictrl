//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2022 Michael Hasling
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

#include <string>
#include <string_view>
#include <vector>

#include "text_logger.h"

namespace xmidictrl {

/**
 * Abstract interface class used by the XMidiCtrl core classes to access
 * services provided by the hosting application (XPlane plugin or stdio tool).
 */

struct inbound_task;

class app_settings {
public:
    virtual bool log_midi() const = 0;
    virtual int max_midi_messages() const = 0;
};

class simcmd_interface {
public:
    virtual void begin(text_logger& in_log, std::string_view in_cmd) = 0;
    virtual void end(text_logger& in_log, std::string_view in_cmd) = 0;

    virtual void execute(text_logger& in_log, std::string_view in_cmd) = 0;
};

class simvar_access {
public:
    virtual bool check(std::string_view in_name) = 0;

    virtual bool read(text_logger& in_log, std::string_view in_name, std::string& out_value) = 0;
    virtual bool read(text_logger& in_log, std::string_view in_name, float& out_value) = 0;
    virtual bool read(text_logger& in_log, std::string_view in_name, std::vector<float>& out_values) = 0;
    virtual bool read(text_logger& in_log, std::string_view in_name, std::vector<int>& out_values) = 0;

    virtual bool write(text_logger& in_log, std::string_view in_name, std::string_view in_value) = 0;
    virtual bool write(text_logger& in_log, std::string_view in_name, float in_value) = 0;

    virtual std::string toggle(text_logger& in_log, std::string_view in_name, std::string_view in_value_on, std::string_view in_value_off) = 0;
};

class app_services {
public:
    virtual ~app_services() = default;

    virtual std::string find_profile(text_logger& profile_log) = 0;

    virtual simcmd_interface& cmd() = 0;
    virtual simvar_access& datarefs() = 0;

    virtual void add_inbound_task(const std::shared_ptr<inbound_task> &in_task) = 0;
    virtual void show_info_message(std::string_view in_id, std::string_view in_msg, int in_seconds = -1) = 0;
};

} // Namespace xmidictrl
