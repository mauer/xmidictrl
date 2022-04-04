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

#ifndef IMGUIWINDOW_H
#define IMGUIWINDOW_H

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

class ImGuiWindow : public xplane_window {
public:
    ImGuiWindow(text_logger &in_log, xplane &in_xp, int in_width, int in_height, bool in_translucent = false);
    ~ImGuiWindow() override;

protected:
    void on_draw() override;
    bool on_click(int x, int y, XPLMMouseStatus status) override;
    bool on_mouse_wheel(int x, int y, int wheel, int clicks) override;
    XPLMCursorStatus on_cursor(int x, int y) override;
    void on_key(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus) override;

    virtual void create_widgets() = 0;

private:
    const float IMGUI_FONT_SIZE = 18.0f;

    static std::shared_ptr<ImGuiFontAtlas> m_imGuiFontAtlas;

    GLuint m_fontTextureId {};
    ImGuiContext *imGuiContext {};
    int mLeft {}, mTop {}, mRight {}, mBottom {};
    bool stopped = false;

    void buildWindow();
    void showWindow();

    void translateImguiToBoxel(float inX, float inY, int &outX, int &outY);
    void translateToImguiSpace(int inX, int inY, float &outX, float &outY);
};

} // Namespace XMidiCtrl

#endif // IMGUIWINDOW_H