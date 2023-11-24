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

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

// Mapping data type
enum class map_data_1_type {
    none,
    control_change,
    note,
    pitch_bend,
    program_change
};




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class map : public std::enable_shared_from_this<map> {
public:
    explicit map(environment& in_env);
    virtual ~map() = default;

    void set_no(unsigned int in_no);
    unsigned int no() const;

    void set_include_name(std::string_view in_name);
    std::string_view include_name() const;

    [[nodiscard]] unsigned char channel() const;
    [[nodiscard]] map_data_1_type data_1_type() const;
    [[nodiscard]] unsigned char data_1() const;

    std::string data_1_as_string();

    [[nodiscard]] std::string_view sl() const;

    std::string_view source_line() const;

    std::string_view map_text(bool in_short = false);

    std::string get_key();

    virtual bool check(text_logger& in_log);

protected:
    environment& env() const;

    void read_common_config(text_logger& in_log, toml::value& in_data, bool in_read_sl = true);

    bool check_sublayer(std::string_view in_sl_value);

    virtual std::string build_mapping_text(bool in_short) = 0;

private:
    void read_channel(text_logger& in_log, toml::value& in_data);
    void read_data_1(text_logger& in_log, toml::value& in_data);
    void read_sublayer(text_logger& in_log, toml::value& in_data);

    std::string data_1_type_as_string();

    static constexpr std::string_view c_cfg_ch {"ch"};
    static constexpr std::string_view c_cfg_cc {"cc"};
    static constexpr std::string_view c_cfg_note {"note"};
    static constexpr std::string_view c_cfg_sl {"sl"};

    environment& m_env;

    unsigned int m_no {0};
    std::string m_include_name {};

    unsigned char m_channel {MIDI_NONE};

    map_data_1_type m_data_1_type {map_data_1_type::none};
    unsigned char m_data_1 {MIDI_NONE};

    std::string m_sl {};

    std::string m_source_line {};

    std::string m_map_text_long {};
    std::string m_map_text_short {};
};

} // Namespace xmidictrl

#endif // XMC_MAP_H
