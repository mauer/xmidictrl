//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#ifndef _XPLANE_H_
#define _XPLANE_H_

// Standard
#include <memory>
#include <string>

// XMidiCtrl
#include "commands.h"
#include "data.h"

namespace xmidictrl {

class xplane {
public:
    xplane();
    ~xplane() = default;

    typedef std::shared_ptr<xplane> ptr;

    XPLMPluginID plugin_id();

    std::string_view plugin_path();
    std::string_view xplane_path();
    std::string_view preferences_path();
    std::string_view profiles_path();

    static std::string current_aircraft_path();
    std::string current_aircraft_author();
    std::string current_aircraft_icao();
    std::string current_aircraft_descr();

    commands &cmd();
    data &datarefs();

private:
    XPLMPluginID m_plugin_id {-1};

    std::string m_xplane_path {};
    std::string m_plugin_path {};
    std::string m_preferences_path {};
    std::string m_profiles_path {};

    std::unique_ptr<commands> m_commands;
    std::unique_ptr<data> m_datarefs;
};

} // namespace xmidictrl

#endif // _XPLANE_H_