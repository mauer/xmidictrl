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

#ifndef SETTINGS_H
#define SETTINGS_H

// Standard
#include <memory>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "config.h"
#include "midi_logger.h"
#include "text_logger.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class settings : public config {
public:
    explicit settings(text_logger &in_text_log, midi_logger &in_midi_log, xplane &in_xp);
    ~settings() override;

    void set_debug_mode(bool in_mode);
    [[nodiscard]] bool debug_mode() const;

    void set_log_midi(bool in_enabled);
    [[nodiscard]] bool log_midi() const;

    void set_show_messages(bool in_enabled);
    [[nodiscard]] bool show_messages() const;

    void set_max_text_messages(int in_number);
    [[nodiscard]] int max_text_messages() const;

    void set_max_midi_messages(int in_number);
    [[nodiscard]] int max_midi_messages() const;

    void set_common_profile(bool in_enabled);
    [[nodiscard]] bool common_profile() const;

    void save_settings();

private:
    std::string get_settings_filename();

    text_logger &m_text_log;
    midi_logger &m_midi_log;
};

} // Namespace xmidictrl

#endif // SETTINGS_H