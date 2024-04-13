/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "SceneView.h"

#include <imgui.h>

#include "IconFontDefines.h"
#include "render/BufferObject.h"
#include "utility/FileSystem.h"
#include "utility/Debug.h"
#include "utility/Utils.h"

#ifndef M_PI
#   define M_PI 		3.1415926535897932384626433832795f
#   define M_2_PI 		6.28318530717958647692528676655901f		// PI*2
#endif


namespace nhahn
{
    SceneView::SceneView(std::shared_ptr<Texture> t)
        : _srcD(t), _screenSize(400, 225)
    {
        _srcSize = glm::vec2(t->width(), t->height());
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        // global gl stats
        glEnable(GL_DEPTH_TEST);
        // create new render target
        _rt = std::make_unique<RenderTarget>();

        // gpu progs
        _quadProg = std::make_unique<Shader>(path.c_str(), "common.inc", "quad.vert", nullptr, "quad.frag", 1);

        // create blank buffer object
        BufferObject* blackPbo = new BufferObject(PIXEL_UNPACK_BUFFER, (GLsizei)(_srcSize.x * _srcSize.y * sizeof(float)*4));
        blackPbo->bind();
        blackPbo->map();
        memset(blackPbo->data(), 0, _srcSize.x * _srcSize.y * sizeof(float) * 4);
        blackPbo->unmap();
        blackPbo->unbind();

        _screen = std::make_unique<Texture>(TEXTURE_2D, _srcSize.x, _srcSize.y);
        _screen->setFormat(TEXEL_FLOAT, 4, 4);
        _screen->init();
        _screen->copyPbo(*blackPbo);

        float* data = new float[_srcSize.x * _srcSize.y * 4];
        for (unsigned int y = 0, idx = 0; y < _srcSize.y; y++) {
            for (unsigned int x = 0; x < _srcSize.x; x++, idx++) {
                data[idx] = 0.5f;
                if (x == _srcSize.x - 1 || y == _srcSize.y - 1)
                    data[idx] = 0.0f;
            }
        }
        _screen->copy(data);
        delete[] data;

        DBG("SceneView", DebugLevel::DEBUG, "Texture memory usage: %dmb\n", (int)(Texture::memoryUsage() / (1024 * 1024)));
    }

    SceneView::~SceneView()
    {
        _screen.reset();
        _quadProg.reset();
        _rt.reset();
    }

    void SceneView::render(double dt)
    {
        _currentFPS = (int)std::round(1.0 / dt);

        // scene Window
        ImGuiWindowFlags screenflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
        //ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos(), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2((float)_screenSize.x, (float)_screenSize.y), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_MDI_EYE " Scene View", nullptr, screenflags);
        ImGui::PopStyleVar(3);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _screenSize = { viewportPanelSize.x, viewportPanelSize.y };

        // get relative mouse position
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 scrPos = ImGui::GetCursorScreenPos();
        ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / _screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / _screenSize.y);

        // render source textures to screen texture
        _rt->bind();
        _rt->pushViewport(0, 0, _srcSize.x, _srcSize.y);
        _srcD->bindAny();
        RtAttachment dst = _rt->attachTextureAny(*_screen);
        _rt->selectAttachmentList(1, dst);
        _quadProg->bind();
        _quadProg->setUniformI("D", _srcD->boundUnit());
        _quadProg->setUniformF("mousePos", glm::vec2(relMousePos.x, relMousePos.y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _quadProg->unbind();
        _rt->popViewport();
        _rt->unbind();

        // add rendered texture to ImGUI scene window
        uint64_t textureID = _screen->glName();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ (float)_screenSize.x, (float)_screenSize.y }, ImVec2{0,1}, ImVec2{1,0});
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(7);

        // add stats info
        {
            char statsLabel[64];
            snprintf(statsLabel, sizeof statsLabel, ICON_MDI_SPEEDOMETER " : %i fps", _currentFPS);
            ImVec2 labelSize = ImGui::CalcTextSize(statsLabel);

            ImGuiWindowFlags statsInfo_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

            const float statsInfo_margin = 10.0f;
            const ImVec2 statsInfo_pad = ImVec2(10.0f, 4.0f);
            
            ImVec2 statsInfo_size = ImVec2(labelSize.x + statsInfo_pad.x * 2, labelSize.y + statsInfo_pad.y * 2);

            ImGui::SameLine();
            ImVec2 statsInfo_pos;
            statsInfo_pos.x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - statsInfo_margin;
            statsInfo_pos.y = ImGui::GetCursorScreenPos().y + statsInfo_margin;

            ImGui::SetNextWindowPos(statsInfo_pos, ImGuiCond_Always, ImVec2(1.0f, 0.0f));
            statsInfo_flags |= ImGuiWindowFlags_NoMove;

            ImGui::SetNextWindowBgAlpha(0.15f); // Transparent background
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, statsInfo_pad);

            if (ImGui::BeginChild("StatsInfo", statsInfo_size, true, statsInfo_flags))
            {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), statsLabel);
            }
            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }
        ImGui::End();
    }
}