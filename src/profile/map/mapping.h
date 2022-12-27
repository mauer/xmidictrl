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

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "text_logger.h"
#include "xmc_types.h"
#include "app_services.h"

namespace xmidictrl {

class mapping : public std::enable_shared_from_this<mapping> {
public:
    explicit mapping(app_services &in_app);
    virtual ~mapping() = default;

    virtual map_type type();

    [[nodiscard]] unsigned char channel() const;
    [[nodiscard]] map_data_type data_type() const;
    [[nodiscard]] unsigned char data() const;

    std::string_view source_line() const;

    std::string_view as_text();

    std::string get_key();

    virtual bool check(text_logger &in_log);

protected:
    app_services &app() const;

    void read_common_config(text_logger &in_log, toml::value &in_data);

    void read_channel(text_logger &in_log, toml::value &in_data);
    void read_data(text_logger &in_log, toml::value &in_data);

    virtual std::string build_mapping_text() = 0;

private:
    app_services &m_app;

    unsigned char m_channel {MIDI_NONE};

    map_data_type m_data_type {map_data_type::none};
    unsigned char m_data {MIDI_NONE};

    std::string m_source_line {};
    std::string m_mapping_text {};
};

} // Namespace xmidictrl
