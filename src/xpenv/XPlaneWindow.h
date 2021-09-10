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

#ifndef XPLANEWINDOW_H
#define XPLANEWINDOW_H

// Standard
#include <cstdint>
#include <functional>
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

namespace XPEnv {

    class XPlaneWindow {
    public:
        XPlaneWindow(int width, int height, int decoration = xplm_WindowDecorationRoundRectangle);
        virtual ~XPlaneWindow();

        XPLMWindowID windowID();

        void show();
        void hide();

        void setTitle(const std::string& title);

        bool isVisible() const;

        //using DrawCallback = std::function<void(XPlaneWindow&)>;
        using ClickCallback = std::function<void(XPlaneWindow &, int, int, XPLMMouseStatus)>;
        using CloseCallback = std::function<void(XPlaneWindow &)>;
        using KeyCallback = std::function<void(XPlaneWindow &, char, char, XPLMKeyFlags)>;


        //void setDrawCallback(DrawCallback cb);
        void setClickCallback(ClickCallback cb);
        void setCloseCallback(CloseCallback cb);
        void setKeyCallback(KeyCallback cb);

        //void setPosition(int posx, int posy);


        //void moveFromOrToVR();
        bool hasKeyboardFocus();
        void requestKeyboardFocus(bool request);

        //void reportClose();
        void boxelsToNative(int x, int y, int &outX, int &outY);

        //bool isPopped();
        //bool isFront();
        //void bringToFront();
        //void setResizingLimits(int minwidth, int minheight, int maxwidth, int maxheight);
        //void setPositioningMode(int pmode, int monindex);
        //void setGravity(float inLeftGravity, float inTopGravity, float inRightGravity, float inBottomGravity);
        //void setWindowGeometry(int mleft, int mtop, int mright, int mbot);
        //void setWindowGeometryOS(int mleft, int mtop, int mright, int mbot);
        //void setWindowGeometryVR(int bWidth, int bHeight);
        //bool isVR();
        //void setIsCmdVisible(int fCondition);
        //bool getIsCmdVisible();

        static void MultiMatrixVec4f(GLfloat dst[4], const GLfloat m[16], const GLfloat v[4]);
    protected:
        virtual void onDraw() = 0;
        virtual bool onClick(int x, int y, XPLMMouseStatus status);
        virtual bool onRightClick(int x, int y, XPLMMouseStatus status);
        virtual void onKey(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus);
        virtual XPLMCursorStatus onCursor(int x, int y);
        virtual bool onMouseWheel(int x, int y, int wheel, int clicks);

        void updateMatrices();
    private:
        void createWindow();

        XPLMWindowID m_windowID;

        int m_width;
        int m_height;

        int m_decoration;

        //bool isInVR = false;

        //bool isCmdVisible = false;

        static XPLMDataRef m_vrEnabledRef;
        static XPLMDataRef m_modelviewMatrixRef;
        static XPLMDataRef m_viewportRef;
        static XPLMDataRef m_projectionMatrixRef;

        float m_modelView[16]{};
        float m_projection[16]{};

        int m_viewport[4]{};

        //DrawCallback onDrawCB;
        ClickCallback onClickCB;
        CloseCallback onCloseCB;
        KeyCallback onKeyCB;
    };

} // Namespace XPEnv

#endif //XPLANEWINDOW_H