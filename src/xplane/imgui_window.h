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
//   Portions copies from Floating Windows with imgui integration for FlyWithLua
//   Copyright (C) 2018 Folke Will <folko@solhost.org>
//   Released as public domain code.
//---------------------------------------------------------------------------------------------------------------------
//   Portions copied from https://github.com/kuroneko/xsb_public
//   ImgWindow.h
//
//   Integration for dear imgui into X-Plane.
//
//   Copyright (C) 2018, Christopher Collins
//---------------------------------------------------------------------------------------------------------------------

#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

// Standord
#include <memory>
#include <string>

// OpenGL
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
    #include <GL/glext.h>

#endif

// Dear ImGui
#include "imgui.h"

// XMidiCtrl
#include "ImGuiFontAtlas.h"
#include "xplane.h"
#include "xplane_window.h"

namespace xmidictrl {

class imgui_window : public xplane_window {
public:
    imgui_window(text_logger &in_log,
                 xplane &in_xp,
                 int in_width,
                 int in_height,
                 window_position in_position = window_position::top_left,
                 int in_offset_x = 100,
                 int in_offset_y = 100,
                 bool in_translucent = false);
    ~imgui_window() override;

protected:
    void on_draw() override;
    bool on_click(int in_x, int in_y, XPLMMouseStatus in_status) override;
    bool on_mouse_wheel(int in_x, int in_y, int in_wheel, int in_clicks) override;
    XPLMCursorStatus on_cursor(int in_x, int in_y) override;
    void on_key(char in_key, XPLMKeyFlags in_flags, char in_virtual_key, bool in_losing_focus) override;

    virtual void create_widgets() = 0;

private:
    void build_window();
    void show_window();

    void translate_imgui_to_boxel(float in_x, float in_y, int &out_x, int &out_y) const;
    void translate_to_imgui_space(int in_x, int in_y, float &out_x, float &out_y) const;

    const float IMGUI_FONT_SIZE = 18.0f;

    static std::shared_ptr<ImGuiFontAtlas> m_font;

    //GLuint m_font_texture_id {};
    int m_font_texture_id {};
    ImGuiContext *m_context {nullptr};

    int m_left {0};
    int m_top {0};
    int m_right {0};
    int m_bottom {0};

    bool m_active {true};
};

} // Namespace xmidictrl

#endif // IMGUI_WINDOW_H