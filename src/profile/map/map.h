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

#ifndef XMC_MAP_H
#define XMC_MAP_H

// Standard
#include <memory>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "environment.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class map : public std::enable_shared_from_this<map> {
public:
    explicit map(environment& in_env);
    virtual ~map() = default;

    virtual map_type type();

    [[nodiscard]] unsigned char channel() const;
    [[nodiscard]] map_data_type data_type() const;
    [[nodiscard]] unsigned char data() const;

    [[nodiscard]] std::string_view sl() const;

    std::string_view source_line() const;

    std::string_view as_text();

    std::string get_key();

    virtual bool check(text_logger& in_log);

protected:
    environment& env() const;

    void read_common_config(text_logger& in_log, toml::value& in_data);

    bool check_sublayer(std::string_view in_sl_value);

    virtual std::string build_mapping_text() = 0;

private:
    void read_channel(text_logger& in_log, toml::value& in_data);
    void read_data(text_logger& in_log, toml::value& in_data);
    void read_sublayer(text_logger& in_log, toml::value& in_data);

    static constexpr std::string_view CFG_KEY_CC {"cc"};
    static constexpr std::string_view CFG_KEY_CH {"ch"};

    environment& m_env;

    unsigned char m_channel {MIDI_NONE};

    map_data_type m_data_type {map_data_type::none};
    unsigned char m_data {MIDI_NONE};

    std::string m_sl {};

    std::string m_source_line {};
    std::string m_mapping_text {};
};

} // Namespace xmidictrl

#endif // XMC_MAP_H
