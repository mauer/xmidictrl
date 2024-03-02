//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

#ifndef XMC_MIDI_MESSAGE_H
#define XMC_MIDI_MESSAGE_H

// Standard
#include <chrono>
#include <vector>

// XMidiCtrl
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   TYPES
//---------------------------------------------------------------------------------------------------------------------

// MIDI direction
enum class midi_direction
{
	in,
	out
};


// MIDI message type
enum class midi_msg_type
{
	aftertouch,
	channel_pressure,
	control_change,
	note_off,
	note_on,
	pitch_bend,
	program_change,
	none
};


const char* const sharp_note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const char* const flat_note_names[] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};




//---------------------------------------------------------------------------------------------------------------------
//   CLASS
//---------------------------------------------------------------------------------------------------------------------

class midi_message {
public:
	midi_message(text_logger& in_log, midi_direction in_direction);

	text_logger& log();

	void clear();

	bool parse_message(std::vector<unsigned char>* in_msg);
	void create_cc_message(unsigned char in_channel, unsigned char in_data, unsigned char in_value);

	bool check();

	[[nodiscard]] unsigned int mapping_count() const;

	std::string mapping_text() const;
	void add_mapping_text(std::string_view in_map_text);

	void set_time(time_point in_time);
	[[nodiscard]] std::string time() const;

	void set_port(unsigned int in_port);
	[[nodiscard]] unsigned int port() const;

	[[nodiscard]] midi_direction direction() const;

	void set_status(unsigned char in_status);
	[[nodiscard]] unsigned char status() const;

	void set_data_1(unsigned char in_data_1);
	[[nodiscard]] char data_1() const;
	[[nodiscard]] std::string data_1_as_text(note_name_type in_type) const;

	void set_data_2(unsigned char in_data_2);
	[[nodiscard]] char data_2() const;

	[[nodiscard]] char channel() const;

	[[nodiscard]] midi_msg_type type() const;
	[[nodiscard]] std::string type_as_text() const;
	[[nodiscard]] std::string type_as_code() const;

private:
	std::unique_ptr<text_logger> m_log;

	std::string m_time {};
	unsigned int m_port {0};

	midi_direction m_direction {midi_direction::in};

	char m_status {MIDI_NONE};
	char m_data_1 {MIDI_NONE};
	char m_data_2 {MIDI_NONE};

	unsigned int m_mapping_count {0};
	std::string m_mapping_text {};
};

} // Namespace xmidictrl

#endif // XMC_MIDI_MESSAGE_H