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
//---------------------------------------------------------------------------------------------------------------------
//   Portions copies from Floating Windows with imgui integration for FlyWithLua
//   Copyright (C) 2018 Folke Will <folko@solhost.org>
//   Released as public domain code.
//---------------------------------------------------------------------------------------------------------------------
//   Portions copied from https://github.com/kuroneko/xsb_public
//   ImgWindow.h
//
//   Integration for Dear ImGui into X-Plane.
//
//   Copyright (C) 2018, Christopher Collins
//---------------------------------------------------------------------------------------------------------------------

// Standard
#include <cctype>

// Fonts
#include "IconsFontAwesome6.h"
#include "fa-solid-900.inc"

// X-Plane SDK
#include <XPLMGraphics.h>

// XMidiCtrl
#include "imgui_window.h"
#include "version.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

std::shared_ptr<ImGuiFontAtlas> imgui_window::s_font = nullptr;


/**
 * Constructor
 */
imgui_window::imgui_window(text_logger& in_log,
                           environment& in_env,
                           int in_width,
                           int in_height,
                           window_position in_position,
                           int in_offset_x,
                           int in_offset_y,
                           bool in_translucent)
    : xplane_window(in_log, in_env, in_width, in_height, in_position, in_offset_x, in_offset_y, in_translucent)
{
    // font atlas for Dear ImGui
    if (!s_font) {
        s_font = std::make_shared<ImGuiFontAtlas>();

        if (!s_font->AddFontFromFileTTF("./Resources/fonts/DejaVuSans.ttf", IMGUI_FONT_SIZE))
            in_log.warn("Error loading font 'DejaVuSans' for Dear ImGui");

        // merge icons
        ImFontConfig config;
        config.MergeMode = true;
        config.PixelSnapH = true;

        static ImVector<ImWchar> icon_ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddText(ICON_FA_ARROW_LEFT
                        ICON_FA_ARROW_RIGHT
                        ICON_FA_ARROW_RIGHT_FROM_BRACKET
                        ICON_FA_ARROW_RIGHT_TO_BRACKET
                        ICON_FA_SQUARE_XMARK
                        ICON_FA_TRIANGLE_EXCLAMATION
                        ICON_FA_CIRCLE_CHECK
                        ICON_FA_CHECK
                        ICON_FA_CHECK_DOUBLE
                        ICON_FA_INFO
                        ICON_FA_TRASH_CAN
                        ICON_FA_FLOPPY_DISK
                        ICON_FA_ROTATE_RIGHT);
        builder.BuildRanges(&icon_ranges);

        if (!s_font->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data,
                                                    fa_solid_900_compressed_size,
                                                    IMGUI_FONT_SIZE,
                                                    &config,
                                                    icon_ranges.Data))
            in_log.warn("Error loading 'Font Awesome' for Dear ImGui");
    }

    // check if a font atlas was supplied
    if (s_font) {
        s_font->bindTexture();
        m_context = ImGui::CreateContext(s_font->getAtlas());
    } else {
        m_context = ImGui::CreateContext();
    }

    // set context for ImGui
    ImGui::SetCurrentContext(m_context);

    // disable window rounding since the frame gets rendered by X-Plane
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 0;

    // Disable ImGui ini-file
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // io.OptMacOSXBehaviors = false;
    // disable OSX-like keyboard behaviours always - we don't have the keymapping for it.
    io.ConfigMacOSXBehaviors = false; // This is the new version above Imgui 1.60
    io.ConfigFlags = ImGuiConfigFlags_NavNoCaptureKeyboard;

    io.KeyMap[ImGuiKey_Tab] = XPLM_VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = XPLM_VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = XPLM_VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = XPLM_VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = XPLM_VK_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = XPLM_VK_PRIOR;
    io.KeyMap[ImGuiKey_PageDown] = XPLM_VK_NEXT;
    io.KeyMap[ImGuiKey_Home] = XPLM_VK_HOME;
    io.KeyMap[ImGuiKey_End] = XPLM_VK_END;
    io.KeyMap[ImGuiKey_Insert] = XPLM_VK_INSERT;
    io.KeyMap[ImGuiKey_Delete] = XPLM_VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = XPLM_VK_BACK;
    io.KeyMap[ImGuiKey_Space] = XPLM_VK_SPACE;
    io.KeyMap[ImGuiKey_Enter] = XPLM_VK_ENTER;
    io.KeyMap[ImGuiKey_Escape] = XPLM_VK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = XPLM_VK_A;
    io.KeyMap[ImGuiKey_C] = XPLM_VK_C;
    io.KeyMap[ImGuiKey_V] = XPLM_VK_V;
    io.KeyMap[ImGuiKey_X] = XPLM_VK_X;
    io.KeyMap[ImGuiKey_Y] = XPLM_VK_Y;
    io.KeyMap[ImGuiKey_Z] = XPLM_VK_Z;

    // bind our font
    if (s_font) {
        m_font_texture_id = static_cast<int>(reinterpret_cast<intptr_t>(io.Fonts->TexID));
    } else {
        uint8_t* pixels;
        int font_tex_width, font_tex_height;
        io.Fonts->GetTexDataAsAlpha8(&pixels, &font_tex_width, &font_tex_height);

        XPLMGenerateTextureNumbers(&m_font_texture_id, 1);

        XPLMBindTexture2d(m_font_texture_id, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_ALPHA,
                     font_tex_width,
                     font_tex_height,
                     0,
                     GL_ALPHA,
                     GL_UNSIGNED_BYTE,
                     pixels);
        io.Fonts->TexID = (void*) (intptr_t) (m_font_texture_id);
    }
}


/**
 * Destructor
 */
imgui_window::~imgui_window()
{
    ImGui::DestroyContext(m_context);

    glDeleteTextures(1, reinterpret_cast<const GLuint*>(&m_font_texture_id));

    //s_font.reset();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Returns the normal text color
 */
ImVec4 imgui_window::text_color()
{
    return ImColor(255, 255, 255);
}


/**
 * Returns the text color of values
 */
ImVec4 imgui_window::value_color()
{
    return ImColor(255, 127, 39);
}


/**
 * Returns the text color of titles
 */
ImVec4 imgui_window::title_color()
{
    return ImColor(75, 160, 255);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Draw the Dear ImGui window
 */
void imgui_window::on_draw()
{
    if (!m_active) {
        return;
    }

    update_matrices();

    try {
        build_window();
        show_window();
    } catch (const std::exception& ex) {
        log().error("Error drawing Dear ImGui window:");
        log().error(ex.what());

        m_active = false;
    }

    ImGui::SetCurrentContext(m_context);

    auto& io = ImGui::GetIO();
    bool focus = has_keyboard_focus();

    if (io.WantTextInput && !focus) {
        request_keyboard_focus(true);
    } else if (!io.WantTextInput && focus) {
        request_keyboard_focus(false);
        // reset keysdown otherwise we'll think any keys used to defocus the keyboard are still down!
        std::fill(std::begin(io.KeysDown), std::end(io.KeysDown), false);
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Build the Dear ImGui window
 */
void imgui_window::build_window()
{
    ImGui::SetCurrentContext(m_context);
    auto& io = ImGui::GetIO();

    // transfer the window geometry to ImGui
    XPLMGetWindowGeometry(window_id(), &m_left, &m_top, &m_right, &m_bottom);

    if (translucent()) {
        XPLMSetGraphicsState(
            0 /* no fog */,
            0 /* 0 texture units */,
            0 /* no lighting */,
            0 /* no alpha testing */,
            1 /* do alpha blend */,
            1 /* do depth testing */,
            0 /* no depth writing */
        );

        XPLMDrawTranslucentDarkBox(m_left, m_top, m_right, m_bottom);
    }

    auto win_width = static_cast<float>(m_right - m_left);
    auto win_height = static_cast<float>(m_top - m_bottom);

    io.DisplaySize = ImVec2(win_width, win_height);
    // in boxels, we're always scale 1, 1.
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2((float) 0.0, (float) 0.0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_Always);

    // and construct the window
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    if (translucent())
        flags = flags | ImGuiWindowFlags_NoBackground;

    ImGui::Begin(XMIDICTRL_NAME, nullptr, flags);

    // create custom widgets
    create_widgets();

    ImGui::End();
    ImGui::Render();
}


/**
 * Show the window
 */
void imgui_window::show_window()
{
    ImGui::SetCurrentContext(m_context);
    auto& io = ImGui::GetIO();

    ImDrawData* draw_data = ImGui::GetDrawData();

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // We are using the OpenGL fixed pipeline because messing with the
    // shader-state in X-Plane is not very well documented, but using the fixed
    // function pipeline is.
    // 1TU + Alpha settings, no depth, no fog.
    XPLMSetGraphicsState(0, 1, 0, 1, 1, 0, 0);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glDisable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(static_cast<GLfloat>(m_left), static_cast<GLfloat>(-m_top), 0.0f);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;

        glVertexPointer(2,
                        GL_FLOAT,
                        sizeof(ImDrawVert),
                        (const GLvoid*) ((const char*) vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2,
                          GL_FLOAT,
                          sizeof(ImDrawVert),
                          (const GLvoid*) ((const char*) vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4,
                       GL_UNSIGNED_BYTE,
                       sizeof(ImDrawVert),
                       (const GLvoid*) ((const char*) vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                glBindTexture(GL_TEXTURE_2D, (GLuint) (intptr_t) pcmd->TextureId);

                // Scissors work in viewport space - must translate the coordinates from ImGui -> Boxels, then Boxels -> Native.
                //FIXME: it must be possible to apply the scale+transform manually to the projection matrix so we don't need to doublestep.
                int boxel_top, boxel_left, boxel_right, boxel_bottom;
                translate_imgui_to_boxel(pcmd->ClipRect.x, pcmd->ClipRect.y, boxel_left, boxel_top);
                translate_imgui_to_boxel(pcmd->ClipRect.z, pcmd->ClipRect.w, boxel_right, boxel_bottom);
                int native_top, native_left, native_right, native_bottom;
                boxels_to_native(boxel_left, boxel_top, native_left, native_top);
                boxels_to_native(boxel_right, boxel_bottom, native_right, native_bottom);
                glScissor(native_left, native_bottom, native_right - native_left, native_top - native_bottom);
                glDrawElements(GL_TRIANGLES,
                               (GLsizei) pcmd->ElemCount,
                               sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                               idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    // Restore modified state
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopAttrib();
    glPopClientAttrib();
}


bool imgui_window::on_click(int in_x, int in_y, XPLMMouseStatus in_status)
{
    ImGui::SetCurrentContext(m_context);
    ImGuiIO& io = ImGui::GetIO();

    float my_x, my_y;
    translate_to_imgui_space(in_x, in_y, my_x, my_y);
    io.MousePos = ImVec2(my_x, my_y);

    switch (in_status) {
        case xplm_MouseDown:
        case xplm_MouseDrag:
            io.MouseDown[0] = true;
            break;

        case xplm_MouseUp:
            io.MouseDown[0] = false;
            break;

        default:
            break;
    }

    return xplane_window::on_click(in_x, in_y, in_status);
}


bool imgui_window::on_mouse_wheel(int in_x, int in_y, int in_wheel, int in_clicks)
{
    ImGui::SetCurrentContext(m_context);
    ImGuiIO& io = ImGui::GetIO();

    float my_x, my_y;
    translate_to_imgui_space(in_x, in_y, my_x, my_y);

    io.MousePos = ImVec2(my_x, my_y);
    switch (in_wheel) {
        case 0:
            io.MouseWheel = static_cast<float>(in_clicks);
            break;

        case 1:
            io.MouseWheelH = static_cast<float>(in_clicks);
            break;

        default:
            break;
    }

    return xplane_window::on_mouse_wheel(in_x, in_y, in_wheel, in_clicks);
}


XPLMCursorStatus imgui_window::on_cursor(int in_x, int in_y)
{
    ImGui::SetCurrentContext(m_context);
    ImGuiIO& io = ImGui::GetIO();

    float my_x, my_y;
    translate_to_imgui_space(in_x, in_y, my_x, my_y);
    io.MousePos = ImVec2(my_x, my_y);

    return xplm_CursorDefault;
}


void imgui_window::on_key(char in_key, XPLMKeyFlags in_flags, char in_virtual_key, bool in_losing_focus)
{
    if (in_losing_focus) {
        return;
    }

    ImGui::SetCurrentContext(m_context);
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantTextInput) {
        // If you press and hold a key, the flags will actually be down, 0, 0, ..., up
        // So the key always has to be considered as pressed unless the up flag is set
        auto vk = static_cast<unsigned char>(in_virtual_key);
        io.KeysDown[vk] = (in_flags & xplm_UpFlag) != xplm_UpFlag;
        io.KeyShift = (in_flags & xplm_ShiftFlag) == xplm_ShiftFlag;
        io.KeyAlt = (in_flags & xplm_OptionAltFlag) == xplm_OptionAltFlag;
        io.KeyCtrl = (in_flags & xplm_ControlFlag) == xplm_ControlFlag;

        if ((in_flags & xplm_UpFlag) != xplm_UpFlag
            && !io.KeyCtrl
            && !io.KeyAlt
            && std::isprint(in_key)) {
            char smallStr[] = {in_key, 0};
            io.AddInputCharactersUTF8(smallStr);
        }
    }

    build_window();

    xplane_window::on_key(in_key, in_flags, in_virtual_key, in_losing_focus);
}


void imgui_window::translate_imgui_to_boxel(float in_x, float in_y, int& out_x, int& out_y) const
{
    out_x = m_left + static_cast<int>(in_x);
    out_y = m_top - static_cast<int>(in_y);
}


void imgui_window::translate_to_imgui_space(int in_x, int in_y, float& out_x, float& out_y) const
{
    out_x = static_cast<float>(in_x - m_left);
    if (out_x < 0.0f || out_x > (float) (m_right - m_left)) {
        out_x = -FLT_MAX;
        out_y = -FLT_MAX;
        return;
    }
    out_y = static_cast<float>(m_top - in_y);
    if (out_y < 0.0f || out_y > (float) (m_top - m_bottom)) {
        out_x = -FLT_MAX;
        out_y = -FLT_MAX;
        return;
    }
}

} // Namespace xmidictrl
