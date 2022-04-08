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
//   Parts copied from https://github.com/sparker256/FlyWithLua/tree/master/src/FloatingWindows
//
//   Floating Windows with imgui integration for FlyWithLua
//   Copyright (C) 2018 Folke Will <folko@solhost.org>
//   Released as public domain code.
//---------------------------------------------------------------------------------------------------------------------

#ifndef XPLANE_WINDOW_H
#define XPLANE_WINDOW_H

// Standard
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// OpenGL
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

// X-Plane SDK
#include <XPLMDataAccess.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>

// XMidiCtrl
#include "text_logger.h"
#include "xplane.h"

namespace xmidictrl {

class xplane_window {
public:
    xplane_window(text_logger &in_log, xplane &in_xp, int in_width, int in_height, bool in_translucent = false);
    virtual ~xplane_window();

    static void multi_matrix_vec4f(GLfloat in_dst[4], const std::vector<float> &in_m, const GLfloat in_v[4]);

    XPLMWindowID window_id();

    void show();
    void hide();

    void set_title(std::string_view in_title);

    [[nodiscard]] bool is_visible() const;

    bool has_keyboard_focus();
    void request_keyboard_focus(bool in_request);

    void boxels_to_native(int in_x, int in_y, int &out_x, int &out_y);

protected:
    virtual void on_draw() = 0;
    virtual bool on_click(int in_x, int in_y, XPLMMouseStatus in_status);
    virtual bool on_right_click(int in_x, int in_y, XPLMMouseStatus in_status);
    virtual void on_key(char in_key, XPLMKeyFlags in_flags, char in_virtual_key, bool in_losing_focus);
    virtual XPLMCursorStatus on_cursor(int in_x, int in_y);
    virtual bool on_mouse_wheel(int in_x, int in_y, int in_wheel, int in_clicks);

    void update_matrices();

    text_logger &m_log;
    xplane &m_xp;

private:
    void create_window(bool in_translucent);

    XPLMWindowID m_window_id {nullptr};

    int m_width {};
    int m_height {};

    std::vector<float> m_modelview {};
    std::vector<float> m_projection {};

    std::vector<int> m_viewport {};
};

} // Namespace xmidictrl

#endif // XPLANE_WINDOW_H