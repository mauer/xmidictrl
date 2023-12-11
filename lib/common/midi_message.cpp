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

#include "midi_message.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
midi_message::midi_message(text_logger& in_log, midi_direction in_direction)
    : m_direction(in_direction)
{
    // create my own logger and set the given logger as parent
    m_log = std::make_unique<text_logger>(&in_log);

    // set debug level if enabled
    m_log->set_debug_mode(in_log.debug_mode());
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the message log
 */
text_logger& midi_message::log()
{
    return *m_log;
}


/**
 * Clear all data
 */
void midi_message::clear()
{
    m_log->clear();

    m_time.clear();
    m_port = 0;

    m_status = MIDI_NONE;
    m_data_1 = MIDI_NONE;
    m_data_2 = MIDI_NONE;
}


/**
 * Parse the given message
 */
bool midi_message::parse_message(std::vector<unsigned char>* in_msg)
{
    clear();

    if (in_msg->size() <= 1) {
        m_log->error("Invalid MIDI message (size <= 1)");
        return false;
    }

    m_status = static_cast<char>(in_msg->at(0));

    // for pitch bend data 1 is always 0
    if (type() == midi_msg_type::pitch_bend)
        m_data_1 = 0;
    else
        m_data_1 = static_cast<char>(in_msg->at(1));

    if (in_msg->size() > 2)
        m_data_2 = static_cast<char>(in_msg->at(2));

    return true;
}


/**
 * Create a midi message
 */
void midi_message::create_cc_message(unsigned char in_channel, unsigned char in_data, unsigned char in_value)
{
    clear();

    m_status = (char) (0xb0 | (in_channel - 1));
    m_data_1 = static_cast<char>(in_data);
    m_data_2 = static_cast<char>(in_value);

    this->set_time(std::chrono::system_clock::now());
}


/**
 * Check the MIDI message
 */
bool midi_message::check()
{
    // check midi type
    switch (type()) {
        case midi_msg_type::aftertouch:
            m_log->error("Invalid MIDI type, 'Aftertouch' messages are not supported");
            return false;

        case midi_msg_type::channel_pressure:
            m_log->error("Invalid MIDI type, 'Channel Pressure' messages are not supported");
            return false;

        case midi_msg_type::none:
            m_log->error(fmt::format("Could not determine MIDI type from Status '{}'", m_status));
            return false;

        default:
            break;
    }

    return true;
}


/**
 * Return the number of mappings
 */
unsigned int midi_message::mapping_count() const
{
    return m_mapping_count;
}


/**
 * Return a string containing all mapping lines
 */
std::string midi_message::mapping_text()
{
    return m_mapping_text;
}


/**
 * Add a mapping text to the midi message
 */
void midi_message::add_mapping_text(std::string_view in_map_text)
{
    m_mapping_text.append(in_map_text);
    m_mapping_count++;
}


/**
 * The the message time
 */
void midi_message::set_time(time_point in_time)
{
    m_time = utils::time_to_string(in_time);
}


/**
 * Return the message time as text
 */
std::string midi_message::time() const
{
    return m_time;
}


/**
 * Set the message port
 */
void midi_message::set_port(unsigned int in_port)
{
    m_port = in_port;
}


/**
 * Return the message port
 */
unsigned int midi_message::port() const
{
    return m_port;
}


/**
 * Return the message direction
 */
midi_direction midi_message::direction() const
{
    return m_direction;
}


/**
 * Set the message status
 */
void midi_message::set_status(unsigned char in_status)
{
    m_status = static_cast<char>(in_status);
}


/**
 * Return the message status
 */
unsigned char midi_message::status() const
{
    return m_status;
}


/**
 * Set the message data 1
 */
void midi_message::set_data_1(unsigned char in_data_1)
{
    m_data_1 = static_cast<char>(in_data_1);
}


/*
 * Return the message data 1
 */
char midi_message::data_1() const
{
    return m_data_1;
}


/**
 * Return the message data 1 as text
 */
std::string midi_message::data_1_as_text(note_name_type in_type) const
{
    std::string str = std::to_string(m_data_1);

    switch (type()) {
        case midi_msg_type::note_off:
        case midi_msg_type::note_on:
            if (in_type == note_name_type::sharp)
                str.append(" (" + std::string(sharp_note_names[static_cast<int>(m_data_1) % 12]) + ")");
            else
                str.append(" (" + std::string(flat_note_names[static_cast<int>(m_data_1) % 12]) + ")");
            break;

        default:
            break;
    }

    return str;
}


/**
 * Set the message data 1
 */
void midi_message::set_data_2(unsigned char in_data_2)
{
    m_data_2 = static_cast<char>(in_data_2);
}


/*
 * Return the message data 2
 */
char midi_message::data_2() const
{
    return m_data_2;
}


// Return the channel
char midi_message::channel() const
{
    if ((m_status & 0xf0) != 0xf0)
        return static_cast<char>((m_status & 0xf) + 1);
    else
        return MIDI_NONE;
}


/**
 * Return the MIDI type
 */
midi_msg_type midi_message::type() const
{
    switch (m_status & 0xf0) {
        case 0xb0:
            return midi_msg_type::control_change;

        case 0x90:
            return midi_msg_type::note_on;

        case 0x80:
            return midi_msg_type::note_off;

        case 0xc0:
            return midi_msg_type::program_change;

        case 0xa0:
            return midi_msg_type::aftertouch;

        case 0xd0:
            return midi_msg_type::channel_pressure;

        case 0xe0:
            return midi_msg_type::pitch_bend;

        default:
            return midi_msg_type::none;
    }
}


/**
 * Return the MIDI type as text
 */
std::string midi_message::type_as_text() const
{
    switch (type()) {
        case midi_msg_type::control_change:
            return "Control Change";

        case midi_msg_type::note_on:
            return "Note On";

        case midi_msg_type::note_off:
            return "Note Off";

        case midi_msg_type::program_change:
            return "Program Change";

        case midi_msg_type::aftertouch:
            return "Aftertouch";

        case midi_msg_type::channel_pressure:
            return "Channel Pressure";

        case midi_msg_type::pitch_bend:
            return "Pitch Bend";

        default:
            break;
    }

    return "<unknown";
}


/**
 * Return the MIDI type as code
 */
std::string midi_message::type_as_code() const
{
    switch (type()) {
        case midi_msg_type::control_change:
            return KEY_CONTROL_CHANGE;

        case midi_msg_type::note_on:
        case midi_msg_type::note_off:
            return KEY_NOTE;

        case midi_msg_type::program_change:
            return KEY_PROGRAM_CHANGE;

        case midi_msg_type::pitch_bend:
            return KEY_PITCH_BEND;

        default:
            break;
    }

    return "";
}

}// Namespace xmidictrl
