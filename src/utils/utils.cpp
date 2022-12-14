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

#include "utils.h"

// Standard
#include <filesystem>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create all required preference folders
 */
bool utils::create_preference_folders(text_logger &in_log, xplane &in_xp)
{
    // check preference folder
    if (!std::filesystem::exists(in_xp.preferences_path())) {
        in_log.info("Directory '" + in_xp.preferences_path() + "' not found");

        if (std::filesystem::create_directory(in_xp.preferences_path())) {
            in_log.info("Directory '" + in_xp.preferences_path() + "' created");
        } else {
            in_log.error("Could not create directory '" + in_xp.preferences_path() + "'");
            return false;
        }
    }

    // check profiles folder
    if (!std::filesystem::exists(in_xp.profiles_path())) {
        in_log.info("Directory '" + in_xp.profiles_path() + "' not found");

        if (std::filesystem::create_directory(in_xp.profiles_path())) {
            in_log.info("Directory '" + in_xp.profiles_path() + "' created");
        } else {
            in_log.error("Could not create directory '" + in_xp.profiles_path() + "'");
            return false;
        }
    }

    return true;
}

} // Namespace xmidictrl
