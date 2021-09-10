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

// X-Plane Environment
#include "XPlaneWindow.h"

namespace XPEnv {

class ImGuiWindow: public XPlaneWindow {
public:
    ImGuiWindow(int width, int height, int decoration = xplm_WindowDecorationRoundRectangle);
    ~ImGuiWindow() override;

    using BuildCallback = std::function<void(ImGuiWindow &)>;
    using ErrorHandler = std::function<void(const std::string &)>;

    void setErrorHandler(ErrorHandler eh);
    void setBuildCallback(BuildCallback cb);

protected:
    void onDraw() override;
    bool onClick(int x, int y, XPLMMouseStatus status) override;
    bool onMouseWheel(int x, int y, int wheel, int clicks) override;
    XPLMCursorStatus onCursor(int x, int y) override;
    void onKey(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus) override;

    virtual void createWidgets() = 0;

private:
    GLuint m_fontTextureId{};
    ImGuiContext *imGuiContext{};
    int mLeft{}, mTop{}, mRight{}, mBottom{};
    ErrorHandler onError;
    BuildCallback doBuild;
    bool stopped = false;

    void buildGUI();
    void showGUI();

    void translateImguiToBoxel(float inX, float inY, int &outX, int &outY);
    void translateToImguiSpace(int inX, int inY, float &outX, float &outY);
};

} // Namespace XPEnv

#endif // IMGUIWINDOW_H