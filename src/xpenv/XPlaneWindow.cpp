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

// Standard
#include <memory>
#include <stdexcept>

// X-Plane SDK
#include <XPLMDataAccess.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <XPLMUtilities.h>

// X-Plane Environment
#include "utils/Logger.h"
#include "XPlaneWindow.h"

namespace XPEnv {

XPLMDataRef XPlaneWindow::m_vrEnabledRef = nullptr;
XPLMDataRef XPlaneWindow::m_modelviewMatrixRef = nullptr;
XPLMDataRef XPlaneWindow::m_viewportRef = nullptr;
XPLMDataRef XPlaneWindow::m_projectionMatrixRef = nullptr;

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
XPlaneWindow::XPlaneWindow(int width, int height, int decoration) :
        m_windowID(nullptr),
        m_width(width),
        m_height(height),
        m_decoration(decoration) {
    // Set global variables
    if (m_vrEnabledRef == nullptr)
        m_vrEnabledRef = XPLMFindDataRef("sim/graphics/VR/enabled");

    if (m_modelviewMatrixRef == nullptr)
        m_modelviewMatrixRef = XPLMFindDataRef("sim/graphics/view/modelview_matrix");

    if (m_viewportRef == nullptr)
        m_viewportRef = XPLMFindDataRef("sim/graphics/view/viewport");

    if (m_projectionMatrixRef == nullptr)
        m_projectionMatrixRef = XPLMFindDataRef("sim/graphics/view/projection_matrix");

    // create window in X-Plane, but don't show it yet
    createWindow();
}


/**
 * Destructor
 */
XPlaneWindow::~XPlaneWindow() {
    // Destroy the window in X-Plane
    if (m_windowID != nullptr)
        XPLMDestroyWindow(m_windowID);
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the window ID
 */
XPLMWindowID XPlaneWindow::windowID() {
    return m_windowID;
}


/**
 * Show the window in X-Plane
 */
void XPlaneWindow::show() {
    if (m_windowID != nullptr)
        XPLMSetWindowIsVisible(m_windowID, true);
}


/**
 * Hide the window in X-Plane
 */
void XPlaneWindow::hide() {
    if (m_windowID != nullptr)
        XPLMSetWindowIsVisible(m_windowID, false);
}


/**
 * Set the title of the window
 */
void XPlaneWindow::setTitle(const std::string& title) {
    if (m_windowID != nullptr)
        XPLMSetWindowTitle(m_windowID, title.c_str());
}


/**
 * Return if the window is visible
 */
bool XPlaneWindow::isVisible() const {
    if (m_windowID != nullptr)
        return XPLMGetWindowIsVisible(m_windowID);
    else
        return false;
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC STATIC
//---------------------------------------------------------------------------------------------------------------------

void XPlaneWindow::MultiMatrixVec4f(GLfloat dst[4], const GLfloat m[16], const GLfloat v[4]) {
    dst[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
    dst[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
    dst[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
    dst[3] = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

bool XPlaneWindow::onClick(int x, int y, XPLMMouseStatus status) {
    if (onClickCB) {
        onClickCB(*this, x, y, status);
    }
    return true;
}


bool XPlaneWindow::onRightClick(int x, int y, XPLMMouseStatus status) {
    return true;
}


void XPlaneWindow::onKey(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus) {
    if (losingFocus) {
        return;
    }

    if (onKeyCB) {
        onKeyCB(*this, key, virtualKey, flags);
    }
}


XPLMCursorStatus XPlaneWindow::onCursor(int x, int y) {
    // Always return the default cursor
    return xplm_CursorDefault;
}

bool XPlaneWindow::onMouseWheel(int x, int y, int wheel, int clicks) {
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create the window
 */
void XPlaneWindow::createWindow() {
    // get the screen boundaries
    int screenLeft, screenTop, screenRight, screenBottom;
    XPLMGetScreenBoundsGlobal(&screenLeft, &screenTop, &screenRight, &screenBottom);

    // construct creation parameters for window
    XPLMCreateWindow_t params;
    params.structSize = sizeof(params);
    params.left = screenLeft + 100;
    params.right = screenLeft + 100 + m_width;
    params.top = screenTop - 100;
    params.bottom = screenTop - 100 - m_height;
    params.visible = 0;
    params.refcon = this;
    params.layer = xplm_WindowLayerFloatingWindows;
    params.decorateAsFloatingWindow = m_decoration;

    // add callbacks for every event
    params.drawWindowFunc = [](XPLMWindowID id, void *ref) {
        reinterpret_cast<XPlaneWindow*>(ref)->onDraw();
    };

    params.handleMouseClickFunc = [](XPLMWindowID id, int x, int y, XPLMMouseStatus status, void* ref) -> int {
        return reinterpret_cast<XPlaneWindow*>(ref)->onClick(x, y, status);
    };

    params.handleRightClickFunc = [](XPLMWindowID id, int x, int y, XPLMMouseStatus status, void* ref) -> int {
        return reinterpret_cast<XPlaneWindow*>(ref)->onRightClick(x, y, status);
    };

    params.handleKeyFunc = [](XPLMWindowID id, char key, XPLMKeyFlags flags, char vKey, void* ref, int losingFocus) {
        reinterpret_cast<XPlaneWindow*>(ref)->onKey(key, flags, vKey, losingFocus);
    };
    params.handleCursorFunc = [](XPLMWindowID id, int x, int y, void* ref) -> XPLMCursorStatus {
        return reinterpret_cast<XPlaneWindow*>(ref)->onCursor(x, y);
    };
    params.handleMouseWheelFunc = [](XPLMWindowID id, int x, int y, int wheel, int clicks, void* ref) -> int {
        return reinterpret_cast<XPlaneWindow*>(ref)->onMouseWheel(x, y, wheel, clicks);
    };

    // create the window
    m_windowID = XPLMCreateWindowEx(&params);

    if (m_windowID == nullptr) {
        LOG_ERROR << "Could not create window in X-Plane" << LOG_END
    }
}


//void XPlaneWindow::setDrawCallback(DrawCallback cb) {
//    onDrawCB = cb;
//}


void XPlaneWindow::setClickCallback(ClickCallback cb) {
    onClickCB = cb;
}


void XPlaneWindow::setCloseCallback(CloseCallback cb) {
    onCloseCB = cb;
}

void XPlaneWindow::setKeyCallback(KeyCallback cb) {
    onKeyCB = cb;
}


/*
void XPlaneWindow::setPosition(int posx, int posy) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled) {
        int winLeft, winTop, winRight, winBot;
        XPLMGetScreenBoundsGlobal(&winLeft, &winTop, &winRight, &winBot);

        XPLMSetWindowGeometry(window, winLeft + posx, winTop - posy, winLeft + posx + width,
                              winTop - posy - height);
    }
}*/

/*
void XPlaneWindow::reportClose() {
    if (onCloseCB) {
        onCloseCB(*this);
    }
}
*/


/*
bool XPlaneWindow::isPopped() {
    return XPLMWindowIsPoppedOut(window);
}

bool XPlaneWindow::isFront() {
    return XPLMIsWindowInFront(window);
}

void XPlaneWindow::bringToFront() {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled) {
        XPLMBringWindowToFront(window);
    }
}

void XPlaneWindow::setResizingLimits(int minwidth, int minheight, int maxwidth, int maxheight) {
    XPLMSetWindowResizingLimits(window, minwidth, minheight, maxwidth, maxheight);
}

void XPlaneWindow::setPositioningMode(int pmode, int monindex) {
    XPLMSetWindowPositioningMode(window, static_cast<XPLMWindowPositioningMode>(pmode), monindex);
}

void
XPlaneWindow::setGravity(float inLeftGravity, float inTopGravity, float inRightGravity, float inBottomGravity) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled) {
        XPLMSetWindowGravity(window, inLeftGravity, inTopGravity, inRightGravity, inBottomGravity);
    }
}

void XPlaneWindow::setWindowGeometry(int mleft, int mtop, int mright, int mbot) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled) {
        XPLMSetWindowGeometry(window, mleft, mtop, mright, mbot);
    }
}

void XPlaneWindow::setWindowGeometryOS(int mleft, int mtop, int mright, int mbot) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled && isPopped()) {
        XPLMSetWindowGeometryOS(window, mleft, mtop, mright, mbot);
    }
}

void XPlaneWindow::setWindowGeometryVR(int bWidth, int bHeight) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (vrEnabled) {
        XPLMSetWindowGeometryVR(window, bWidth, bHeight);
    }
}

bool XPlaneWindow::isVR() {
    return XPLMWindowIsInVR(window);
}

void XPlaneWindow::setIsCmdVisible(int fCondition) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled) {
        fCondition == 0 ? isCmdVisible = true : isCmdVisible = false;
    }
}

bool XPlaneWindow::getIsCmdVisible() {
    return isCmdVisible;
}

void XPlaneWindow::moveFromOrToVR() {
    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (vrEnabled && !isInVR) {
        // X-Plane switched to VR but our window isn't in VR
        XPLMSetWindowPositioningMode(window, xplm_WindowVR, -1);
        isInVR = true;
    } else if (!vrEnabled && isInVR) {
        // Our window is still in VR but X-Plane switched to 2D
        XPLMSetWindowPositioningMode(window, xplm_WindowPositionFree, -1);
        isInVR = false;

        int winLeft, winTop, winRight, winBot;
        XPLMGetScreenBoundsGlobal(&winLeft, &winTop, &winRight, &winBot);

        XPLMSetWindowGeometry(window, winLeft + 100, winTop - 100, winLeft + 100 + width, winTop - 100 - height);
    }
}*/


bool XPlaneWindow::hasKeyboardFocus() {
    if (m_windowID != nullptr)
        return XPLMHasKeyboardFocus(m_windowID);
    else
        return false;
}


void XPlaneWindow::requestKeyboardFocus(bool request) {
    if (m_windowID != nullptr)
        XPLMTakeKeyboardFocus(request ? m_windowID : nullptr);
}


void XPlaneWindow::updateMatrices() {
    // Get the current modelview matrix, viewport, and projection matrix from X-Plane
    XPLMGetDatavf(m_modelviewMatrixRef, m_modelView, 0, 16);
    XPLMGetDatavf(m_projectionMatrixRef, m_projection, 0, 16);
    XPLMGetDatavi(m_viewportRef, m_viewport, 0, 4);
}


void XPlaneWindow::boxelsToNative(int x, int y, int &outX, int &outY) {
    GLfloat boxelPos[4] = {(GLfloat) x, (GLfloat) y, 0, 1};
    GLfloat eye[4], ndc[4];

    MultiMatrixVec4f(eye, m_modelView, boxelPos);
    MultiMatrixVec4f(ndc, m_projection, eye);
    ndc[3] = 1.0f / ndc[3];
    ndc[0] *= ndc[3];
    ndc[1] *= ndc[3];

    outX = static_cast<int>((ndc[0] * 0.5f + 0.5f) * m_viewport[2] + m_viewport[0]);
    outY = static_cast<int>((ndc[1] * 0.5f + 0.5f) * m_viewport[3] + m_viewport[1]);
}

} // Namespace XPEnv