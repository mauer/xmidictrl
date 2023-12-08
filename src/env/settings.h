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

#ifndef XMC_SETTINGS_H
#define XMC_SETTINGS_H

// Standard
#include <filesystem>
#include <memory>

// toml11
#include <imgui.h>
#include <toml.hpp>

// ImGui
//#include <imgui.h>

// XMidiCtrl
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class settings {
public:
    explicit settings(text_logger& in_text_log, std::filesystem::path in_path);
    ~settings() = default;

    void set_debug_mode(bool in_enabled);
    [[nodiscard]] bool debug_mode() const;

    void set_log_midi(bool in_enabled);
    [[nodiscard]] bool log_midi() const;

    void set_show_errors(bool in_enabled);
    [[nodiscard]] bool show_errors() const;

    void set_virtual_channel(int in_channel);
    [[nodiscard]] int virtual_channel() const;

    void set_default_outbound_delay(float in_delay);
    [[nodiscard]] float default_outbound_delay() const;

    void set_max_text_messages(int in_number);
    [[nodiscard]] int max_text_messages() const;

    void set_max_midi_messages(int in_number);
    [[nodiscard]] int max_midi_messages() const;

    void set_default_text_sort(sort_mode in_mode);
    [[nodiscard]] sort_mode default_text_sort() const;

    void set_default_midi_sort(sort_mode in_mode);
    [[nodiscard]] sort_mode default_midi_sort() const;

    void set_note_name(note_name_type in_type);
    [[nodiscard]] note_name_type note_name() const;

    void set_use_common_profile(bool in_enabled);
    [[nodiscard]] bool use_common_profile() const;

    void set_info_disabled(bool in_disabled);
    [[nodiscard]] bool info_disabled() const;

    void set_info_position(window_position in_position);
    [[nodiscard]] window_position info_position() const;

    void set_info_offset_x(int in_offset);
    [[nodiscard]] int info_offset_x() const;

    void set_info_offset_y(int in_offset);
    [[nodiscard]] int info_offset_y() const;

    void set_info_seconds(int in_seconds);
    [[nodiscard]] int info_seconds() const;

    void set_title_color(ImVec4 in_color);
    [[nodiscard]] ImVec4 title_color() const;

    void set_value_color(ImVec4 in_color);
    [[nodiscard]] ImVec4 value_color() const;

    void save_settings();

private:
    std::string get_settings_filename();

    text_logger& m_text_log;

    std::filesystem::path m_settings_path {};
    toml::value m_settings_file {};

    bool m_debug_mode {false};
    bool m_log_midi {true};
    bool m_show_errors {true};

    int m_virtual_channel {16};
    float m_default_outbound_delay {0.5f};

    int m_max_text_messages {1500};
    int m_max_midi_messages {150};

    note_name_type m_note_name {note_name_type::sharp};

    sort_mode m_default_text_sort {sort_mode::ascending};
    sort_mode m_default_midi_sort {sort_mode::ascending};

    bool m_use_common_profile {true};

    bool m_info_disabled {false};

    window_position m_info_position {window_position::bottom_left};
    int m_info_offset_x {50};
    int m_info_offset_y {50};
    int m_info_seconds {3};

    ImVec4 m_value_color {ImColor(255, 127, 39)};
    ImVec4 m_title_color {ImColor(75, 160, 255)};
};

} // Namespace xmidictrl

#endif // XMC_SETTINGS_H