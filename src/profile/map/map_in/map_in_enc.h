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

#ifndef MAP_IN_ENC_H
#define MAP_IN_ENC_H

// Standard
#include <string>
#include <string_view>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "map_in.h"
#include "text_logger.h"
#include "midi_message.h"

namespace xmidictrl {

class map_in_enc : public map_in {
public:
    explicit map_in_enc(xplane &in_xp);
    ~map_in_enc() override = default;

    map_type type() override;

    void set_mode(encoder_mode in_mode);
    [[nodiscard]] encoder_mode mode() const;

    void set_dataref(std::string_view in_dataref);
    [[nodiscard]] std::string_view dataref() const;

    void set_modifier_up(float in_modifier);
    [[nodiscard]] float modifier_up() const;

    void set_modifier_down(float in_modifier);
    [[nodiscard]] float modifier_down() const;

    void set_modifier_fast_up(float in_modifier);
    [[nodiscard]] float modifier_fast_up() const;

    void set_modifier_fast_down(float in_modifier);
    [[nodiscard]] float modifier_fast_down() const;

    void set_command_up(std::string_view in_command_up);
    [[nodiscard]] std::string_view command_up() const;

    void set_command_fast_up(std::string_view in_command_fast_up);
    [[nodiscard]] std::string_view command_fast_up() const;

    void set_command_down(std::string_view in_command_down);
    [[nodiscard]] std::string_view command_down() const;

    void set_command_fast_down(std::string_view in_command_fast_down);
    [[nodiscard]] std::string_view command_fast_down() const;

    void read_config(text_logger &in_log, toml::value &in_data) override;
    bool check(text_logger &in_log) override;

    bool execute(midi_message &in_msg, std::string_view in_sl_value) override;

protected:
    std::string build_mapping_text() override;

private:
    bool execute_dataref(midi_message &in_msg);
    bool execute_command(midi_message &in_msg);

    encoder_mode m_mode {encoder_mode::relative};

    unsigned int m_velocity_prev {MIDI_NONE};

    std::string m_dataref {};

    float m_modifier_up {0.0f};
    float m_modifier_down {0.0f};

    float m_modifier_fast_up {0.0f};
    float m_modifier_fast_down {0.0f};

    bool m_value_min_defined {false};
    bool m_value_max_defined {false};

    float m_value_min {0.0f};
    float m_value_max {0.0f};

    std::string m_command_up {};
    std::string m_command_down {};

    std::string m_command_fast_up {};
    std::string m_command_fast_down {};
};

} // Namespace xmidictrl

#endif // MAP_IN_ENC_H
