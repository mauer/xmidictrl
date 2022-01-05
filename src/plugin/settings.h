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
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class settings : public config {
public:
    explicit settings(std::shared_ptr<xplane> xp);
    ~settings() override;

    void set_logging_level(log_level level);
    log_level logging_level() const;

    void set_log_midi(bool enabled);
    bool log_midi() const;

    void set_show_messages(bool enabled);
    bool show_messages() const;

    void set_max_text_messages(int number);
    int max_text_messages() const;

    void set_max_midi_messages(int number);
    int max_midi_messages() const;

    void save_settings();

private:
    std::string get_settings_filename();
};

} // Namespace xmidictrl

#endif // SETTINGS_H