//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   XMidiCtrl - A MIDI Controller plugin for X-Plane 11
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

#include "UserInterface.h"

// Standard
#include <vector>

using namespace XPEnv;


//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
UserInterface::UserInterface() {
    // initialise width and height of the UI
    m_width = 3440; // TODO: Determine current screensize
    m_height = 1440;

    m_buffer.resize(m_width * m_height);

    initialise();
}


/**
 * Destructor
 */
UserInterface::~UserInterface() {

}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create a new window in X-Plane
 * @param title Title of the window
 * @param width Initial width of the window
 * @param height Initial height of the window
 */
void UserInterface::createWindow(const std::string &title, const int width, const int height) {
    XPLMWindowID window;

    // get screen boundaries
    int left, top, right, bottom;
    XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);

    // construct creation parameters for window
    XPLMCreateWindow_t windowParams;
    windowParams.structSize = sizeof(windowParams);
    windowParams.left = left + 100;
    windowParams.right = left + 100 + width;
    windowParams.top = top - 100;
    windowParams.bottom = top - 100 - height;
    windowParams.visible = 1;
    windowParams.refcon = this;
    windowParams.layer = xplm_WindowLayerFloatingWindows;
    windowParams.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

    // add callbacks for every event
    windowParams.drawWindowFunc = [] (XPLMWindowID id, void* ref) {
        reinterpret_cast<UserInterface*>(ref)->onDraw(id);
    };

    windowParams.handleRightClickFunc = [] (XPLMWindowID id, int x, int y, XPLMMouseStatus status, void* ref) -> int{
        return reinterpret_cast<UserInterface*>(ref)->onRightClick(id, x, y, status);
    };

    windowParams.handleKeyFunc = [] (XPLMWindowID id, char key, XPLMKeyFlags flags, char virtualKey, void* ref,
            int losingFocus) {
    };

    windowParams.handleCursorFunc = [] (XPLMWindowID id, int x, int y, void* ref) -> XPLMCursorStatus {
        return reinterpret_cast<UserInterface*>(ref)->onCursor(id, x, y);
    };

    windowParams.handleMouseWheelFunc = [] (XPLMWindowID id, int x, int y, int wheel, int clicks, void* ref) -> int {
        return reinterpret_cast<UserInterface*>(ref)->onMouseWheel(id, x, y, wheel, clicks);
    };

    windowParams.handleMouseClickFunc = [] (XPLMWindowID id, int x, int y, XPLMMouseStatus status, void* ref) -> int {
        return reinterpret_cast<UserInterface*>(ref)->onClick(id, x, y, status);
    };

    // create the window
    window = XPLMCreateWindowEx(&windowParams);

    // set window title
    XPLMSetWindowTitle(window, title.c_str());
}


void UserInterface::onDraw(XPLMWindowID id) {

}


bool UserInterface::onClick(XPLMWindowID id, int x, int y, XPLMMouseStatus status) {
    return true;
}


bool UserInterface::onRightClick(XPLMWindowID id, int x, int y, XPLMMouseStatus status) {
    return true;
}


XPLMCursorStatus UserInterface::onCursor(XPLMWindowID id, int x, int y) {
    return xplm_CursorDefault;
}


bool UserInterface::onMouseWheel(XPLMWindowID id, int x, int y, int wheel, int clicks) {
    return false;
}


//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Initialise the UI framework
 */
void UserInterface::initialise() {
}