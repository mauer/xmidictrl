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
//   Parts copied from https://github.com/sparker256/FlyWithLua/tree/master/src/FloatingWindows
//
//   Floating Windows with imgui integration for FlyWithLua
//   Copyright (C) 2018 Folke Will <folko@solhost.org>
//   Released as public domain code.
//---------------------------------------------------------------------------------------------------------------------

#ifndef XMC_XPLANE_WINDOW_H
#define XMC_XPLANE_WINDOW_H

// Standard
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// OpenGL
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#elif defined(_MSC_VER)
    #include <windows.h>
    #include <GL/gl.h>

#else
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

// X-Plane SDK
#include <XPLMDataAccess.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>

// XMidiCtrl
#include "environment.h"
#include "text_logger.h"
#include "types.h"

namespace xmidictrl {

class xplane_window {
public:
    xplane_window(text_logger& in_log,
                  environment& in_env,
                  int in_width,
                  int in_height,
                  window_position in_position = window_position::top_left,
                  int in_offset_x = 100,
                  int in_offset_y = 100,
                  bool in_translucent = false);
    virtual ~xplane_window();

    static void multi_matrix_vec4f(GLfloat in_dst[4], const std::vector<float>& in_m, const GLfloat in_v[4]);

    text_logger& log();

    environment& env();

    XPLMWindowID window_id();
    [[nodiscard]] bool translucent() const;

    virtual void show();
    void hide();

    void set_window_position(window_position in_position,
                             int in_width,
                             int in_height,
                             int in_offset_x,
                             int in_offset_y);

    void set_title(std::string_view in_title);
    [[nodiscard]] bool is_visible() const;

    bool has_keyboard_focus();
    void request_keyboard_focus(bool in_request);

    void boxels_to_native(int in_x, int in_y, int& out_x, int& out_y);

protected:
    virtual void on_draw() = 0;
    virtual bool on_click(int in_x, int in_y, XPLMMouseStatus in_status);
    virtual bool on_right_click(int in_x, int in_y, XPLMMouseStatus in_status);
    virtual void on_key(char in_key, XPLMKeyFlags in_flags, char in_virtual_key, bool in_losing_focus);
    virtual XPLMCursorStatus on_cursor(int in_x, int in_y);
    virtual bool on_mouse_wheel(int in_x, int in_y, int in_wheel, int in_clicks);

    void update_matrices();

private:
    void calc_window_position(int& out_left, int& out_right, int& out_top, int& out_bottom);

    void create_window();

    text_logger& m_log;

    environment& m_env;
    XPLMWindowID m_window_id {nullptr};

    int m_width {};
    int m_height {};

    window_position m_position {window_position::top_left};
    int m_offset_x {100};
    int m_offset_y {100};

    bool m_translucent {false};

    std::vector<float> m_modelview {};
    std::vector<float> m_projection {};

    std::vector<int> m_viewport {};
};

} // Namespace xmidictrl

#endif // XMC_XPLANE_WINDOW_H