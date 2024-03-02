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

#ifndef XMC_COMMAND_TESTS_H
#define XMC_COMMAND_TESTS_H

// Standard
#include <map>
#include <memory>
#include <string_view>

// XMidiCtrl
#include "commands.h"
#include "text_logger.h"

namespace xmidictrl {

class command_tests : public commands {
public:
    command_tests() = default;
    ~command_tests() override = default;

    void begin(text_logger &in_log, std::string_view in_cmd) override;
    void end(text_logger &in_log, std::string_view in_cmd) override;
    
    void execute(text_logger &in_log, std::string_view in_cmd) override;

    std::string current_command() const;
    std::string last_command() const;

private:
    std::string m_current_command {};
    std::string m_last_command {};
};

} // Namespace xmidictrl

#endif // XMC_COMMAND_TESTS_H