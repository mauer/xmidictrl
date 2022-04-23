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

#include "map_init.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_init::map_init(xplane &in_xp)
    : map(in_xp)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_type map_init::type()
{
    return map_type::none;
}


/**
 * Set the velocity
 */
void map_init::set_velocity(int in_velocity)
{
    if (in_velocity >= MIDI_VELOCITY_MIN && in_velocity <= MIDI_VELOCITY_MAX)
        m_velocity = in_velocity;
    else
        m_velocity = MIDI_VELOCITY_MAX;
}


/**
 * Read settings from config
 */
void map_init::read_config(text_logger &in_log, toml::value &in_data)
{
    in_log.debug(" --> Line %i :: Read settings for init message", in_data.location().line());

    read_common_config(in_log, in_data);

    // read velocity on
    set_velocity(toml_utils::read_int(in_log, in_data, CFG_KEY_VELOCITY, true));
}


/**
 * Check the mapping
 */
bool map_init::check(text_logger &in_log)
{
    bool result = true;

    if (!map::check(in_log))
        result = false;

    if (m_velocity < MIDI_VELOCITY_MIN || m_velocity > MIDI_VELOCITY_MAX) {
        in_log.error(source_line());
        in_log.error(" --> Invalid value for parameter '%s', velocity has to be between 0 and 127",
                     CFG_KEY_VELOCITY);
        result = false;
    }

    return result;
}


/**
 * Create a MIDI outbound task
 */
std::shared_ptr<outbound_task> map_init::execute(text_logger &in_log)
{
    std::shared_ptr<outbound_task> task = std::make_shared<outbound_task>();

    task->data_changed = true;

    switch (data_type()) {
        case map_data_type::control_change:
            task->type = midi_msg_type::control_change;
            break;

        case map_data_type::note:
            if (m_velocity == MIDI_VELOCITY_MAX)
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

    task->ch = channel();
    task->data = data();

    task->velocity = m_velocity;

    // add mapping to task
    task->map = shared_from_this();

    return task;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_init::build_mapping_text()
{
    std::string map_str = " ====== Init Message ======\n";

    // Velocity
    map_str.append("Velocity = '" + std::to_string(m_velocity) + "'\n");

    return map_str;
}


} // Namespace xmidictrl