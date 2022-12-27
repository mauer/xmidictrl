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
#include <string>
#include <vector>

// XMidiCtrl
#include "mapping.h"
#include "text_logger.h"

namespace xmidictrl {

enum class midi_direction {
    in,
    out
};

const char* const sharp_note_names[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
const char* const flat_note_names[]  = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };

class midi_message {
public:
    midi_message(text_logger &in_log, midi_direction in_direction);
    ~midi_message();

    text_logger &log();

    void clear();
    bool parse_message(std::vector<unsigned char> *in_msg);
    bool check();

    [[nodiscard]] size_t mapping_count() const;
    std::string mappings_as_string();
    void add_mapping(const std::shared_ptr<mapping> &in_map);

    void set_time(time_point in_time);
    [[nodiscard]] std::string time() const;

    void set_port(unsigned int in_port);
    [[nodiscard]] unsigned int port() const;

    [[nodiscard]] midi_direction direction() const;

    void set_status(unsigned char in_status);
    [[nodiscard]] unsigned char status() const;

    void set_data_1(unsigned char in_data_1);
    [[nodiscard]] unsigned char data_1() const;
    [[nodiscard]] std::string data_1_as_text(note_name_type in_type) const;

    void set_data_2(unsigned char in_data_2);
    [[nodiscard]] unsigned char data_2() const;

    [[nodiscard]] unsigned char channel() const;

    [[nodiscard]] midi_msg_type type() const;
    [[nodiscard]] std::string type_as_text() const;
    [[nodiscard]] std::string type_as_code() const;

    [[nodiscard]] std::string key() const;

private:
    std::unique_ptr<text_logger> m_log;

    std::string m_time {};
    unsigned int m_port {0};

    midi_direction m_direction {midi_direction::in};

    unsigned char m_status {MIDI_NONE};
    unsigned char m_data_1 {MIDI_NONE};
    unsigned char m_data_2 {MIDI_NONE};

    std::vector<std::shared_ptr<mapping>> m_mappings;
};

} // Namespace xmidictrl
