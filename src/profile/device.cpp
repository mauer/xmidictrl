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

#include "device.h"

// XMidiCtrl
#include "map_in_cmd.h"
#include "conversions.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
device::device(text_logger& in_text_log,
               midi_logger& in_midi_log,
               std::string_view in_name)
    : m_text_log(in_text_log),
      m_midi_log(in_midi_log),
      m_name(in_name)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the device name
 */
std::string_view device::name()
{
    return m_name;
}


/**
 * Add a new inbound mapping to the device
 */
void device::add_inbound_map(std::shared_ptr<map_in>& in_mapping)
{
    m_map_in.add(in_mapping);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the text logger
 */
text_logger& device::text_log()
{
    return m_text_log;
}


/**
 * Return the midi logger
 */
midi_logger& device::midi_log()
{
    return m_midi_log;
}


/**
 * Return the inbound mapping list
 */
map_in_list& device::mapping_in()
{
    return m_map_in;
}

} // Namespace xmidictrl