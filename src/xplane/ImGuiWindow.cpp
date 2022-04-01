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

// Standard
#include <cstdint>
#include <cctype>
#include <utility>

// X-Plane SDK
#include <XPLMGraphics.h>

// XMidiCtrl
#include "ImGuiWindow.h"
#include "version.h"

namespace xmidictrl {

std::shared_ptr<ImGuiFontAtlas> ImGuiWindow::m_imGuiFontAtlas = nullptr;


//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
ImGuiWindow::ImGuiWindow(text_logger *in_log, xplane *in_xp, int in_width, int in_height, bool in_translucent)
    : xplane_window(in_log, in_xp, in_width, in_height, in_translucent)
{
    // font atlas for Dear ImGui
    if (!m_imGuiFontAtlas) {
        m_imGuiFontAtlas = std::make_shared<ImGuiFontAtlas>();

        if (!m_imGuiFontAtlas->AddFontFromFileTTF("./Resources/fonts/DejaVuSans.ttf", IMGUI_FONT_SIZE))
            in_log->warn("Error loading font 'DejaVuSans' for Dear ImGui");

        // merge icons
        ImFontConfig config;
        config.MergeMode = true;

        static ImVector<ImWchar> icon_ranges;
        ImFontGlyphRangesBuilder builder;

        builder.AddText(ICON_FA_ANGLE_DOUBLE_DOWN
        ICON_FA_ANGLE_DOUBLE_UP
            ICON_FA_CAMERA
        ICON_FA_CHECK
            ICON_FA_CHECK_CIRCLE
        ICON_FA_CHEVRON_DOWN
            ICON_FA_CHEVRON_UP
        ICON_FA_CLIPBOARD_LIST
            ICON_FA_EXCLAMATION_TRIANGLE
        ICON_FA_EYE
            ICON_FA_EXTERNAL_LINK_SQUARE_ALT
        ICON_FA_FOLDER_OPEN
            ICON_FA_INFO_CIRCLE
        ICON_FA_LEVEL_UP_ALT
            ICON_FA_PLANE
        ICON_FA_QUESTION_CIRCLE
            ICON_FA_SAVE
        ICON_FA_SEARCH
            ICON_FA_SLIDERS_H
        ICON_FA_SORT
            ICON_FA_SPINNER
        ICON_FA_TIMES
            ICON_FA_TRASH_ALT
        ICON_FA_UNDO
            ICON_FA_UPLOAD
        ICON_FA_WINDOW_CLOSE
            ICON_FA_WINDOW_MAXIMIZE
        ICON_FA_WINDOW_RESTORE);
        builder.BuildRanges(&icon_ranges);
    }

    // check if a font atlas was supplied
    if (m_imGuiFontAtlas) {
        m_imGuiFontAtlas->bindTexture();
        imGuiContext = ImGui::CreateContext(m_imGuiFontAtlas->getAtlas());
    } else
        imGuiContext = ImGui::CreateContext();

    // set context for ImGui
    ImGui::SetCurrentContext(imGuiContext);

    // disable window rounding since the frame gets rendered by X-Plane
    auto &style = ImGui::GetStyle();
    style.WindowRounding = 0;

    // Disable ImGui ini-file
    auto &io = ImGui::GetIO();
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
    if (m_imGuiFontAtlas) {
        m_fontTextureId = static_cast<GLuint>(reinterpret_cast<intptr_t>(io.Fonts->TexID));
    } else {
        uint8_t *pixels;
        int fontTexWidth, fontTexHeight;
        io.Fonts->GetTexDataAsAlpha8(&pixels, &fontTexWidth, &fontTexHeight);

        int textureId;
        XPLMGenerateTextureNumbers(&textureId, 1);
        m_fontTextureId = (GLuint) textureId;

        XPLMBindTexture2d(m_fontTextureId, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, fontTexWidth, fontTexHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
        io.Fonts->TexID = (void *) (intptr_t) (m_fontTextureId);
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Draw the Dear ImGui window
 */
void ImGuiWindow::on_draw()
{
    if (stopped) {
        return;
    }

    update_matrices();

    try {
        buildWindow();
        showWindow();
    } catch (const std::exception &ex) {
        m_log->error("Error drawing Dear ImGui window:");
        m_log->error(ex.what());
        stopped = true;
    }

    ImGui::SetCurrentContext(imGuiContext);

    auto &io = ImGui::GetIO();
    bool focus = hasKeyboardFocus();

    if (io.WantTextInput && !focus) {
        requestKeyboardFocus(true);
    } else if (!io.WantTextInput && focus) {
        requestKeyboardFocus(false);
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
void ImGuiWindow::buildWindow()
{
    ImGui::SetCurrentContext(imGuiContext);
    auto &io = ImGui::GetIO();

    // transfer the window geometry to ImGui
    XPLMGetWindowGeometry(window_id(), &mLeft, &mTop, &mRight, &mBottom);

    float win_width = static_cast<float>(mRight - mLeft);
    float win_height = static_cast<float>(mTop - mBottom);

    io.DisplaySize = ImVec2(win_width, win_height);
    // in boxels, we're always scale 1, 1.
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2((float) 0.0, (float) 0.0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_Always);

    // and construct the window
    ImGui::Begin(XMIDICTRL_NAME,
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // create custom widgets
    create_widgets();

    ImGui::End();
    ImGui::Render();
}


/**
 * Show the window
 */
void ImGuiWindow::showWindow()
{
    ImGui::SetCurrentContext(imGuiContext);
    auto &io = ImGui::GetIO();

    ImDrawData *drawData = ImGui::GetDrawData();

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    drawData->ScaleClipRects(io.DisplayFramebufferScale);

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
    glTranslatef(static_cast<GLfloat>(mLeft), static_cast<GLfloat>(-mTop), 0.0f);

    // Render command lists
    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList *cmd_list = drawData->CmdLists[n];
        const ImDrawVert *vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx *idx_buffer = cmd_list->IdxBuffer.Data;
        glVertexPointer(2,
                        GL_FLOAT,
                        sizeof(ImDrawVert),
                        (const GLvoid *) ((const char *) vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2,
                          GL_FLOAT,
                          sizeof(ImDrawVert),
                          (const GLvoid *) ((const char *) vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4,
                       GL_UNSIGNED_BYTE,
                       sizeof(ImDrawVert),
                       (const GLvoid *) ((const char *) vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                glBindTexture(GL_TEXTURE_2D, (GLuint) (intptr_t) pcmd->TextureId);

                // Scissors work in viewport space - must translate the coordinates from ImGui -> Boxels, then Boxels -> Native.
                //FIXME: it must be possible to apply the scale+transform manually to the projection matrix so we don't need to doublestep.
                int bTop, bLeft, bRight, bBottom;
                translateImguiToBoxel(pcmd->ClipRect.x, pcmd->ClipRect.y, bLeft, bTop);
                translateImguiToBoxel(pcmd->ClipRect.z, pcmd->ClipRect.w, bRight, bBottom);
                int nTop, nLeft, nRight, nBottom;
                boxels_to_native(bLeft, bTop, nLeft, nTop);
                boxels_to_native(bRight, bBottom, nRight, nBottom);
                glScissor(nLeft, nBottom, nRight - nLeft, nTop - nBottom);
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


bool ImGuiWindow::on_click(int x, int y, XPLMMouseStatus status)
{
    ImGui::SetCurrentContext(imGuiContext);
    ImGuiIO &io = ImGui::GetIO();

    float outX, outY;
    translateToImguiSpace(x, y, outX, outY);
    io.MousePos = ImVec2(outX, outY);

    switch (status) {
        case xplm_MouseDown:
        case xplm_MouseDrag:
            io.MouseDown[0] = true;
            break;
        case xplm_MouseUp:
            io.MouseDown[0] = false;
            break;
    }

    return xplane_window::on_click(x, y, status);
}


bool ImGuiWindow::on_mouse_wheel(int x, int y, int wheel, int clicks)
{
    ImGui::SetCurrentContext(imGuiContext);
    ImGuiIO &io = ImGui::GetIO();

    float outX, outY;
    translateToImguiSpace(x, y, outX, outY);

    io.MousePos = ImVec2(outX, outY);
    switch (wheel) {
        case 0:
            io.MouseWheel = static_cast<float>(clicks);
            break;
        case 1:
            io.MouseWheelH = static_cast<float>(clicks);
            break;
    }

    return xplane_window::on_mouse_wheel(x, y, wheel, clicks);
}


XPLMCursorStatus ImGuiWindow::on_cursor(int x, int y)
{
    ImGui::SetCurrentContext(imGuiContext);
    ImGuiIO &io = ImGui::GetIO();

    float outX, outY;
    translateToImguiSpace(x, y, outX, outY);
    io.MousePos = ImVec2(outX, outY);

    return xplm_CursorDefault;
}


void ImGuiWindow::on_key(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus)
{
    if (losingFocus) {
        return;
    }

    ImGui::SetCurrentContext(imGuiContext);
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantTextInput) {
        // If you press and hold a key, the flags will actually be down, 0, 0, ..., up
        // So the key always has to be considered as pressed unless the up flag is set
        auto vk = static_cast<unsigned char>(virtualKey);
        io.KeysDown[vk] = (flags & xplm_UpFlag) != xplm_UpFlag;
        io.KeyShift = (flags & xplm_ShiftFlag) == xplm_ShiftFlag;
        io.KeyAlt = (flags & xplm_OptionAltFlag) == xplm_OptionAltFlag;
        io.KeyCtrl = (flags & xplm_ControlFlag) == xplm_ControlFlag;

        if ((flags & xplm_UpFlag) != xplm_UpFlag
            && !io.KeyCtrl
            && !io.KeyAlt
            && std::isprint(key)) {
            char smallStr[] = {key, 0};
            io.AddInputCharactersUTF8(smallStr);
        }
    }

    buildWindow();

    xplane_window::on_key(key, flags, virtualKey, losingFocus);
}


void ImGuiWindow::translateImguiToBoxel(float inX, float inY, int &outX, int &outY)
{
    outX = (int) (mLeft + inX);
    outY = (int) (mTop - inY);
}


void ImGuiWindow::translateToImguiSpace(int inX, int inY, float &outX, float &outY)
{
    outX = static_cast<float>(inX - mLeft);
    if (outX < 0.0f || outX > (float) (mRight - mLeft)) {
        outX = -FLT_MAX;
        outY = -FLT_MAX;
        return;
    }
    outY = static_cast<float>(mTop - inY);
    if (outY < 0.0f || outY > (float) (mTop - mBottom)) {
        outX = -FLT_MAX;
        outY = -FLT_MAX;
        return;
    }
}


ImGuiWindow::~ImGuiWindow()
{
    ImGui::DestroyContext(imGuiContext);
    glDeleteTextures(1, &m_fontTextureId);

    m_imGuiFontAtlas.reset();
}

} // Namespace XMidiCtrl