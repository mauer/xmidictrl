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

#ifndef MAP_IN_H
#define MAP_IN_H

// Standard
#include <memory>

// XMidiCtrl
#include "environment.h"
#include "label.h"
#include "map.h"
#include "midi_message.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class device;

class map_in : public map {
public:
    explicit map_in(environment &in_env);
    ~map_in() override = default;

    [[nodiscard]] std::string_view sl() const;

    virtual void read_config(text_logger &in_log, toml::value &in_data, device &in_device, toml::value &in_config);

    virtual bool execute(midi_message &in_msg, std::string_view in_sl_value) = 0;

protected:
    bool check_sublayer(std::string_view in_sl_value);
    void toggle_dataref(text_logger &in_log, std::string_view in_dataref, std::vector<std::string> &in_values);

    void display_label(text_logger &in_log, float in_value);
    void display_label(text_logger &in_log, std::string_view in_value);

private:
    void read_sublayer(text_logger &in_log, toml::value &in_data);
    void read_label(text_logger &in_log, toml::value &in_data, toml::value &in_config);

    std::string m_sl {};

    std::unique_ptr<label> m_label {};
};

} // Namespace xmiditrl

#endif // MAP_IN_H
