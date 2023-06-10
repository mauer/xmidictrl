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

#include "map_out_con.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_out_con::map_out_con(environment &in_env)
    : map_out(in_env)
{}


/**
 * Destructor
 */
map_out_con::~map_out_con()
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_out_con::type()
{
    return map_type::constant;
}

/**
 * Set the velocity
 */
void map_out_con::set_velocity(int in_velocity)
{
    if (in_velocity >= MIDI_VELOCITY_MIN && in_velocity <= MIDI_VELOCITY_MAX)
        m_velocity = in_velocity;
    else
        m_velocity = MIDI_VELOCITY_MAX;
}


/**
 * Read settings from config
 */
void map_out_con::read_config(text_logger &in_log, toml::value &in_data)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'con'");

    read_common_config(in_log, in_data);

    // read velocity
    set_velocity(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY, true));
}


/**
 * Check the mapping
 */
bool map_out_con::check(text_logger &in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (m_velocity < MIDI_VELOCITY_MIN || m_velocity > MIDI_VELOCITY_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '" + std::string(CFG_KEY_VELOCITY) + "', "
                     + "velocity has to be between " + std::to_string(MIDI_VELOCITY_MIN) + " and " + std::to_string(MIDI_VELOCITY_MAX));
        result = false;
    }

    return result;
}


/**
 * Create a MIDI outbound task if required
 */
std::shared_ptr<outbound_task> map_out_con::execute(text_logger &in_log, const mode_out in_mode)
{
    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = false;

    switch (data_type()) {
        case map_data_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_type::note:
            if (m_velocity != MIDI_VELOCITY_MIN)
                task->type = midi_msg_type::note_on;
            else
                task->type = midi_msg_type::note_off;
            break;

        case map_data_type::pitch_bend:
            task->type = midi_msg_type::pitch_bend;
            break;

        case map_data_type::program_change:
            task->type = midi_msg_type::program_change;
            break;

        case map_data_type::none:
            task->type = midi_msg_type::none;
            break;
    }

    task->channel = channel();
    task->data = data();
    task->velocity = m_velocity;

    // add mapping to task
    task->mapping = shared_from_this();

    return task;
}


/**
 * Reset the lights on the MIDI device
 */
std::shared_ptr<outbound_task> map_out_con::reset()
{
    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = true;

    switch (data_type()) {
        case map_data_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_type::note:
            task->type = midi_msg_type::note_off;
            break;

        case map_data_type::pitch_bend:
            task->type = midi_msg_type::pitch_bend;
            break;

        case map_data_type::program_change:
            task->type = midi_msg_type::program_change;
            break;

        case map_data_type::none:
            task->type = midi_msg_type::none;
            break;
    }

    task->channel = channel();
    task->data = data();

    task->velocity = MIDI_VELOCITY_MIN;

    return task;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_out_con::build_mapping_text()
{
    std::string map_str = " ====== Constant ======\n";

    // Velocity
    map_str.append("Velocity = '" + std::to_string(m_velocity) + "'\n");

    return map_str;
}

} // Namespace xmidictrl
