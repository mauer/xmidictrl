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

#include "midi_message.h"

// XMidiCtrl
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
midi_message::midi_message(text_logger &in_log, midi_direction in_direction)
    : m_direction(in_direction)
{
    m_log = std::make_unique<text_logger>(&in_log);
    m_log->set_debug_mode(in_log.debug_mode());
}


/**
 * Destructor
 */
midi_message::~midi_message()
{
    m_log.reset();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the message log
 */
text_logger &midi_message::log()
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
bool midi_message::parse_message(std::vector<unsigned char> *in_msg)
{
    clear();

    if (in_msg->size() <= 1) {
        m_log->error("Invalid MIDI message (size <= 1)");
        return false;
    }

    m_status = in_msg->at(0);
    m_data_1 = in_msg->at(1);

    if (in_msg->size() > 2)
        m_data_2 = in_msg->at(2);

    return true;
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
            m_log->error("Could not determine MIDI type from Status '" + std::to_string(m_status) + "'");
            return false;

        default:
            break;
    }

    return true;
}


/**
 * Return the number of mappings
 */
size_t midi_message::mapping_count() const
{
    return m_mappings.size();
}


/**
 * Return a string containing all mapping source lines
 */
std::string midi_message::mappings_as_string()
{
    if (m_mappings.size() == 1)
        return m_mappings.at(0)->as_text().data();

    std::string map_str;
    for (auto &mapping: m_mappings) {
        if (map_str.empty()) {
            map_str = mapping->as_text();
        } else {
            map_str.append("\n");
            map_str.append(mapping->as_text());
        }
    }

    return map_str;
}


/**
 * Add mapping to MIDI message
 */
void midi_message::add_mapping(const std::shared_ptr<map> &in_map)
{
    m_mappings.push_back(in_map);
}


/**
 * The the message time
 */
void midi_message::set_time(time_point in_time)
{
    m_time = conversions::time_to_string(in_time);
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
    m_status = in_status;
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
    m_data_1 = in_data_1;
}


/*
 * Return the message data 1
 */
unsigned char midi_message::data_1() const
{
    return m_data_1;
}


/**
 * Return the message data 1 as text
 */
std::string midi_message::data_1_as_text(note_name_type in_type) const
{
    std::string m_data_1_string = std::to_string(m_data_1);

    switch (type()) {
        case midi_msg_type::note_off:
        case midi_msg_type::note_on:
            if (in_type == note_name_type::sharp)
                m_data_1_string.append(" (" + std::string(sharp_note_names[static_cast<int>(m_data_1) % 12]) + ")");
            else
                m_data_1_string.append(" (" + std::string(flat_note_names[static_cast<int>(m_data_1) % 12]) + ")");
            break;

        default:
            break;
    }

    return m_data_1_string;
}


/**
 * Set the message data 1
 */
void midi_message::set_data_2(unsigned char in_data_2)
{
    m_data_2 = in_data_2;
}


/*
 * Return the message data 2
 */
unsigned char midi_message::data_2() const
{
    return m_data_2;
}


// Return the channel
unsigned char midi_message::channel() const
{
    if ((m_status & 0xf0) != 0xf0)
        return (m_status & 0xf) + 1;
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


/**
 * Return a key containing the channel, type and data
 */
std::string midi_message::key() const
{
    // for pitch bend messages the data will be ignored, it's always 0
    if (type() == midi_msg_type::pitch_bend)
        return conversions::create_map_key(channel(), type_as_code(), 0);
    else
        return conversions::create_map_key(channel(), type_as_code(), m_data_1);
}

} // Namespace xmidictrl
