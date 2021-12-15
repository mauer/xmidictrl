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
#include "xplane.h"

namespace xmidictrl {

class xplane_window {
public:
    xplane_window(std::shared_ptr<xplane> xp, int width, int height, bool translucent = false);
    virtual ~xplane_window();

    typedef std::shared_ptr<xplane_window> ptr;

    //static void multi_matrix_vec4f(GLfloat dst[4], const GLfloat m[16], const GLfloat v[4]);
    static void multi_matrix_vec4f(GLfloat dst[4], const std::vector<float> m, const GLfloat v[4]);

    XPLMWindowID window_id();

    void show();
    void hide();

    void set_title(std::string_view title);

    [[nodiscard]] bool is_visible() const;

    bool hasKeyboardFocus();
    void requestKeyboardFocus(bool request);

    void boxels_to_native(int x, int y, int &outX, int &outY);

 protected:
    virtual void on_draw() = 0;
    virtual bool on_click(int x, int y, XPLMMouseStatus status);
    virtual bool on_right_click(int x, int y, XPLMMouseStatus status);
    virtual void on_key(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus);
    virtual XPLMCursorStatus on_cursor(int x, int y);
    virtual bool on_mouse_wheel(int x, int y, int wheel, int clicks);

    void update_matrices();

    std::shared_ptr<xplane> m_xp;

 private:
    void create_window(bool translucent);

    XPLMWindowID m_window_id {nullptr};

    int m_width;
    int m_height;

    //static XPLMDataRef m_modelviewMatrixRef;
    //static XPLMDataRef m_viewportRef;
    //static XPLMDataRef m_projectionMatrixRef;

    //float m_modelView[16]{};
    //float m_projection[16]{};
    //int m_viewport[4]{};
    std::vector<float> m_modelview {};
    std::vector<float> m_projection {};

    std::vector<int> m_viewport {};
};

} // Namespace XMidiCtrl

#endif // XPLANE_WINDOW_H