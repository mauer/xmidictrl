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
//----------------------------------------------------------------------------------------------------------------------
//   Parts copied from https://github.com/sparker256/FlyWithLua/tree/master/src/FloatingWindows
//
//   Floating Windows with imgui integration for FlyWithLua
//   Copyright (C) 2018 Folke Will <folko@solhost.org>
//   Released as public domain code.
//---------------------------------------------------------------------------------------------------------------------

#include "xplane_window.h"

// Standard
#include <stdexcept>
#include <utility>

// X-Plane SDK
#include <XPLMDataAccess.h>
#include <XPLMDisplay.h>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
xplane_window::xplane_window(std::shared_ptr<xplane> xp, int width, int height, bool translucent)
    : m_xp(std::move(xp)),
      m_width(width),
      m_height(height)
{
    // create window in X-Plane, but don't show it yet
    create_window(translucent);
}


/**
 * Destructor
 */
xplane_window::~xplane_window()
{
    if (m_window_id != nullptr)
        XPLMDestroyWindow(m_window_id);
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return multi matrix
 */
void xplane_window::multi_matrix_vec4f(GLfloat dst[4], const std::vector<float> &m, const GLfloat v[4])
{
    dst[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
    dst[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
    dst[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
    dst[3] = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];
}


/**
 * Return the window ID
 */
XPLMWindowID xplane_window::window_id()
{
    return m_window_id;
}


/**
 * Show the window in X-Plane
 */
void xplane_window::show()
{
    if (m_window_id != nullptr)
        XPLMSetWindowIsVisible(m_window_id, true);
}


/**
 * Hide the window in X-Plane
 */
void xplane_window::hide()
{
    if (m_window_id != nullptr)
        XPLMSetWindowIsVisible(m_window_id, false);
}


/**
 * Set the title of the window
 */
void xplane_window::set_title(std::string_view title)
{
    if (m_window_id != nullptr)
        XPLMSetWindowTitle(m_window_id, title.data());
}


/**
 * Return if the window is visible
 */
bool xplane_window::is_visible() const
{
    if (m_window_id != nullptr)
        return XPLMGetWindowIsVisible(m_window_id);
    else
        return false;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * On click event
 */
bool xplane_window::on_click(int x, int y, XPLMMouseStatus status)
{
    return true;
}


/**
 * On right click event
 */
bool xplane_window::on_right_click(int x, int y, XPLMMouseStatus status)
{
    return true;
}


/**
 * On key pressed event
 */
void xplane_window::on_key(char key, XPLMKeyFlags flags, char virtual_key, bool losing_focus)
{
    if (losing_focus) {
        return;
    }
}


/**
 * On cursor event
 */
XPLMCursorStatus xplane_window::on_cursor(int x, int y)
{
    // always return the default cursor
    return xplm_CursorDefault;
}


/**
 * On mouse wheel event
 */
bool xplane_window::on_mouse_wheel(int x, int y, int wheel, int clicks)
{
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create a new window in X-Plane
 */
void xplane_window::create_window(bool translucent)
{
    // get the X-Plane screen boundaries in boxels
    int screen_left, screen_top, screen_right, screen_bottom;
    XPLMGetScreenBoundsGlobal(&screen_left, &screen_top, &screen_right, &screen_bottom);

    // construct creation parameters for window
    XPLMCreateWindow_t params;
    params.structSize = sizeof(params);
    params.left = screen_left + 100;
    params.right = screen_left + 100 + m_width;
    params.top = screen_top - 100;
    params.bottom = screen_top - 100 - m_height;
    params.visible = 1;
    params.refcon = this;
    params.layer = xplm_WindowLayerFloatingWindows;

    // set window decoration
    if (translucent)
        params.decorateAsFloatingWindow = xplm_WindowDecorationNone;
    else
        params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

    // add callbacks for every event
    params.drawWindowFunc = [](XPLMWindowID id, void *ref) {
        reinterpret_cast<xplane_window *>(ref)->on_draw();
    };

    params.handleMouseClickFunc = [](XPLMWindowID id, int x, int y, XPLMMouseStatus status, void *ref)->int {
        return reinterpret_cast<xplane_window *>(ref)->on_click(x, y, status);
    };

    params.handleRightClickFunc = [](XPLMWindowID id, int x, int y, XPLMMouseStatus status, void *ref)->int {
        return reinterpret_cast<xplane_window *>(ref)->on_right_click(x, y, status);
    };

    params.handleKeyFunc = [](XPLMWindowID id, char key, XPLMKeyFlags flags, char vKey, void *ref, int losingFocus) {
        reinterpret_cast<xplane_window *>(ref)->on_key(key, flags, vKey, losingFocus);
    };
    params.handleCursorFunc = [](XPLMWindowID id, int x, int y, void *ref)->XPLMCursorStatus {
        return reinterpret_cast<xplane_window *>(ref)->on_cursor(x, y);
    };
    params.handleMouseWheelFunc = [](XPLMWindowID id, int x, int y, int wheel, int clicks, void *ref)->int {
        return reinterpret_cast<xplane_window *>(ref)->on_mouse_wheel(x, y, wheel, clicks);
    };

    // create the window
    m_window_id = XPLMCreateWindowEx(&params);

    if (m_window_id == nullptr) {
        LOG_ERROR << "Could not create window in X-Plane" << LOG_END
    }
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
}

void XPlaneWindow::setWindowGeometry(int mleft, int mtop, int mright, int mbot) {

    bool vrEnabled = XPLMGetDatai(vrEnabledRef);

    if (!vrEnabled) {
        XPLMSetWindowGeometry(window, mleft, mtop, mright, mbot);
    }
}*/


bool xplane_window::hasKeyboardFocus()
{
    if (m_window_id != nullptr)
        return XPLMHasKeyboardFocus(m_window_id);
    else
        return false;
}


void xplane_window::requestKeyboardFocus(bool request)
{
    if (m_window_id != nullptr)
        XPLMTakeKeyboardFocus(request ? m_window_id : nullptr);
}


void xplane_window::update_matrices()
{
    // Get the current modelview matrix, viewport, and projection matrix from X-Plane
    m_xp->datarefs().read("sim/graphics/view/modelview_matrix", m_modelview);
    m_xp->datarefs().read("sim/graphics/view/projection_matrix", m_projection);
    m_xp->datarefs().read("sim/graphics/view/viewport", m_viewport);
}


void xplane_window::boxels_to_native(int x, int y, int &out_x, int &out_y)
{
    GLfloat boxelPos[4] = {(GLfloat) x, (GLfloat) y, 0, 1};
    GLfloat eye[4], ndc[4];

    multi_matrix_vec4f(eye, m_modelview, boxelPos);
    multi_matrix_vec4f(ndc, m_projection, eye);
    ndc[3] = 1.0f / ndc[3];
    ndc[0] *= ndc[3];
    ndc[1] *= ndc[3];

    out_x = static_cast<int>((ndc[0] * 0.5f + 0.5f) * m_viewport[2] + m_viewport[0]);
    out_y = static_cast<int>((ndc[1] * 0.5f + 0.5f) * m_viewport[3] + m_viewport[1]);
}

} // Namespace xmidictrl