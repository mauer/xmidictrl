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

// X-Plane SDK
#include <XPLMDisplay.h>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
xplane_window::xplane_window(text_logger& in_log,
                             environment& in_env,
                             int in_width,
                             int in_height,
                             window_position in_position,
                             int in_offset_x,
                             int in_offset_y,
                             bool in_translucent)
    : m_log(in_log),
      m_env(in_env),
      m_width(in_width),
      m_height(in_height),
      m_position(in_position),
      m_offset_x(in_offset_x),
      m_offset_y(in_offset_y),
      m_translucent(in_translucent)
{
    // create window in X-Plane, but don't show it yet
    create_window();
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
void xplane_window::multi_matrix_vec4f(GLfloat in_dst[4], const std::vector<float>& in_m, const GLfloat in_v[4])
{
    in_dst[0] = in_v[0] * in_m[0] + in_v[1] * in_m[4] + in_v[2] * in_m[8] + in_v[3] * in_m[12];
    in_dst[1] = in_v[0] * in_m[1] + in_v[1] * in_m[5] + in_v[2] * in_m[9] + in_v[3] * in_m[13];
    in_dst[2] = in_v[0] * in_m[2] + in_v[1] * in_m[6] + in_v[2] * in_m[10] + in_v[3] * in_m[14];
    in_dst[3] = in_v[0] * in_m[3] + in_v[1] * in_m[7] + in_v[2] * in_m[11] + in_v[3] * in_m[15];
}


/**
 * Return the log
 */
text_logger& xplane_window::log()
{
    return m_log;
}


/**
 * Return the environment
 */
environment& xplane_window::env()
{
    return m_env;
}


/**
 * Return the window ID
 */
XPLMWindowID xplane_window::window_id()
{
    return m_window_id;
}


/**
 * Return if the window should be displayed translucent
 */
bool xplane_window::translucent() const
{
    return m_translucent;
}


/**
 * Show the window in X-Plane
 */
void xplane_window::show()
{
    if (m_window_id != nullptr) {
        if (!is_visible())
            XPLMSetWindowIsVisible(m_window_id, true);
    }
}


/**
 * Hide the window in X-Plane
 */
void xplane_window::hide()
{
    if (m_window_id != nullptr) {
        if (is_visible())
            XPLMSetWindowIsVisible(m_window_id, false);
    }
}


/**
 * Set a new window position
 */
void xplane_window::set_window_position(window_position in_position,
                                        int in_width,
                                        int in_height,
                                        int in_offset_x,
                                        int in_offset_y)
{
    m_position = in_position;
    m_width = in_width;
    m_height = in_height;
    m_offset_x = in_offset_x;
    m_offset_y = in_offset_y;

    int left, right, top, bottom;
    calc_window_position(left, right, top, bottom);

    XPLMSetWindowGeometry(window_id(), left, top, right, bottom);
}


/**
 * Set the title of the window
 */
void xplane_window::set_title(std::string_view in_title)
{
    if (m_window_id != nullptr)
        XPLMSetWindowTitle(m_window_id, in_title.data());
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
bool xplane_window::on_click(int, int, XPLMMouseStatus)
{
    return true;
}


/**
 * On right click event
 */
bool xplane_window::on_right_click(int, int, XPLMMouseStatus)
{
    return true;
}


/**
 * On key pressed event
 */
void xplane_window::on_key(char, XPLMKeyFlags, char, bool)
{
}


/**
 * On cursor event
 */
XPLMCursorStatus xplane_window::on_cursor(int, int)
{
    // always return the default cursor
    return xplm_CursorDefault;
}


/**
 * On mouse wheel event
 */
bool xplane_window::on_mouse_wheel(int, int, int, int)
{
    return true;
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Calculate the a new window position
 */
void xplane_window::calc_window_position(int& out_left, int& out_right, int& out_top, int& out_bottom)
{
    // get the X-Plane screen boundaries in boxels
    int screen_left, screen_top, screen_right, screen_bottom;
    XPLMGetScreenBoundsGlobal(&screen_left, &screen_top, &screen_right, &screen_bottom);

    switch (m_position) {
        case window_position::top_left:
            out_left = screen_left + m_offset_x;
            out_right = out_left + m_width;
            out_top = screen_top - m_offset_y;
            out_bottom = out_top - m_height;
            break;

        case window_position::bottom_left:
            out_left = screen_left + m_offset_x;
            out_right = screen_left + m_offset_x + m_width;
            out_top = screen_bottom + m_offset_y + m_height;
            out_bottom = screen_bottom + m_offset_y;
            break;

        case window_position::top_right:
            out_left = screen_right - m_offset_x - m_width;
            out_right = screen_right - m_offset_x;
            out_top = screen_top - m_offset_y;
            out_bottom = screen_top - m_offset_y - m_height;
            break;

        case window_position::bottom_right:
            out_left = screen_right - m_offset_x - m_width;
            out_right = screen_right - m_offset_x;
            out_top = screen_bottom + m_offset_y + m_height;
            out_bottom = screen_bottom + m_offset_y;
            break;

        case window_position::center:
            out_left = (int) (screen_right - screen_left - m_width) / 2 + m_offset_x;
            out_right = out_left + m_width;
            out_top = (int) (screen_top - screen_bottom - m_height) / 2 + m_offset_y;
            out_bottom = out_top - m_height;
            break;

        case window_position::top_center:
            out_left = (int) (screen_right - screen_left - m_width) / 2;
            out_right = out_left + m_width;
            out_top = screen_top - m_offset_y;
            out_bottom = screen_top - m_offset_y - m_height;
            break;

        case window_position::bottom_center:
            out_left = (int) (screen_right - screen_left - m_width) / 2;
            out_right = out_left + m_width;
            out_top = screen_bottom + m_offset_y + m_height;
            out_bottom = screen_bottom + m_offset_y;
            break;
    }
}


/**
 * Create a new window in X-Plane
 */
void xplane_window::create_window()
{
    // get the X-Plane screen boundaries in boxels
    //int screen_left, screen_top, screen_right, screen_bottom;
    //XPLMGetScreenBoundsGlobal(&screen_left, &screen_top, &screen_right, &screen_bottom);

    // construct creation parameters for window
    XPLMCreateWindow_t params;
    params.structSize = sizeof(params);
    params.visible = 0;
    params.refcon = this;
    params.layer = xplm_WindowLayerFloatingWindows;

    calc_window_position(params.left, params.right, params.top, params.bottom);

    // set window decoration
    if (m_translucent)
        params.decorateAsFloatingWindow = xplm_WindowDecorationNone;
    else
        params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

    // add callbacks for every event
    params.drawWindowFunc = [](XPLMWindowID, void* ref) {
        reinterpret_cast<xplane_window*>(ref)->on_draw();
    };

    params.handleMouseClickFunc = [](XPLMWindowID, int x, int y, XPLMMouseStatus status, void* ref)->int {
        return reinterpret_cast<xplane_window*>(ref)->on_click(x, y, status);
    };

    params.handleRightClickFunc = [](XPLMWindowID, int x, int y, XPLMMouseStatus status, void* ref)->int {
        return reinterpret_cast<xplane_window*>(ref)->on_right_click(x, y, status);
    };

    params.handleKeyFunc = [](XPLMWindowID, char key, XPLMKeyFlags flags, char vKey, void* ref, int losingFocus) {
        reinterpret_cast<xplane_window*>(ref)->on_key(key, flags, vKey, losingFocus);
    };
    params.handleCursorFunc = [](XPLMWindowID, int x, int y, void* ref)->XPLMCursorStatus {
        return reinterpret_cast<xplane_window*>(ref)->on_cursor(x, y);
    };
    params.handleMouseWheelFunc = [](XPLMWindowID, int x, int y, int wheel, int clicks, void* ref)->int {
        return reinterpret_cast<xplane_window*>(ref)->on_mouse_wheel(x, y, wheel, clicks);
    };

    // create the window
    m_window_id = XPLMCreateWindowEx(&params);

    if (m_window_id == nullptr) {
        m_log.error("Could not create window in X-Plane");
    }
}


/**
 * Return if keyboard has the focus
 */
bool xplane_window::has_keyboard_focus()
{
    if (m_window_id != nullptr)
        return XPLMHasKeyboardFocus(m_window_id);
    else
        return false;
}


/**
 * Request keyboard focus
 */
void xplane_window::request_keyboard_focus(bool in_request)
{
    if (m_window_id != nullptr)
        XPLMTakeKeyboardFocus(in_request ? m_window_id : nullptr);
}


/**
 * Update matrices from X-Plane datarefs
 */
void xplane_window::update_matrices()
{
    // Get the current modelview matrix, viewport, and projection matrix from X-Plane
    m_env.drf().read(m_log, "sim/graphics/view/modelview_matrix", m_modelview);
    m_env.drf().read(m_log, "sim/graphics/view/projection_matrix", m_projection);
    m_env.drf().read(m_log, "sim/graphics/view/viewport", m_viewport);
}


/**
 * Convert boxels to native resolution
 */
void xplane_window::boxels_to_native(int in_x, int in_y, int& out_x, int& out_y)
{
    GLfloat boxel_pos[4] = {(GLfloat) in_x, (GLfloat) in_y, 0, 1};
    GLfloat eye[4], ndc[4];

    multi_matrix_vec4f(eye, m_modelview, boxel_pos);
    multi_matrix_vec4f(ndc, m_projection, eye);
    ndc[3] = 1.0f / ndc[3];
    ndc[0] *= ndc[3];
    ndc[1] *= ndc[3];

    out_x = static_cast<int>((ndc[0] * 0.5f + 0.5f) * m_viewport[2] + m_viewport[0]);
    out_y = static_cast<int>((ndc[1] * 0.5f + 0.5f) * m_viewport[3] + m_viewport[1]);
}

} // Namespace xmidictrl