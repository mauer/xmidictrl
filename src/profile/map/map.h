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

#ifndef MAP_H
#define MAP_H

// Standard
#include <memory>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "text_logger.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class map {
public:
    explicit map(xplane *in_xp);
    virtual ~map() = default;

    virtual map_type type();

    [[nodiscard]] unsigned char channel() const;
    [[nodiscard]] map_data_type data_type() const;
    [[nodiscard]] unsigned char data() const;

    std::string_view source_line();

    std::string get_key();

    virtual void read_config(text_logger *in_log, toml::value &in_data);
    virtual bool check(text_logger *in_log);

protected:
    void read_channel(text_logger *in_log, toml::value &in_data);
    void read_data(text_logger *in_log, toml::value &in_data);

    xplane *m_xp;

private:
    unsigned char m_channel {MIDI_NONE};

    map_data_type m_data_type {map_data_type::none};
    unsigned char m_data {MIDI_NONE};

    std::string m_source_line {};
};

} // Namespace xmidictrl

#endif // MAP_H
