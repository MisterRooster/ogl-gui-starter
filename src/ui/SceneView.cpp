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
#include "scene/Scene.h"
#include "utility/FileSystem.h"
#include "utility/Debug.h"
#include "utility/Utils.h"


namespace nhahn
{
    SceneView::SceneView(glm::uvec2 resolution)
        : _screenResolution(resolution), _screenSize(400, 225)
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        // create new render target
        _rt = std::make_shared<RenderTarget>();

        // default shader program
        _defaultProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "default.frag", 1);

        // create blank buffer object
        BufferObject* blackPbo = new BufferObject(PIXEL_UNPACK_BUFFER, (GLsizei)(_screenResolution.x * _screenResolution.y * sizeof(float)*4));
        blackPbo->bind();
        blackPbo->map();
        memset(blackPbo->data(), 0, _screenResolution.x * _screenResolution.y * sizeof(float) * 4);
        blackPbo->unmap();
        blackPbo->unbind();

        // create screen texture to render to
        _screen = std::make_unique<Texture>(TEXTURE_2D, _screenResolution.x, _screenResolution.y);
        _screen->setFormat(TEXEL_FLOAT, 4, 4);
        _screen->init();
        _screen->copyPbo(*blackPbo);

        DBG("SceneView", DebugLevel::DEBUG, "Texture memory usage: %dmb\n", (int)(Texture::memoryUsage() / (1024 * 1024)));
    }

    SceneView::~SceneView()
    {
        // release smart pointers
        _screen.reset();
        _defaultProg.reset();
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

        // set initial window size on first use
        ImGui::SetNextWindowSize(ImVec2((float)_screenSize.x, (float)_screenSize.y), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_MDI_EYE " Scene View", nullptr, screenflags);
        ImGui::PopStyleVar(3);

        // set screensize to viewport size
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _screenSize = { viewportPanelSize.x, viewportPanelSize.y };

        // get relative mouse position
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 scrPos = ImGui::GetCursorScreenPos();
        ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / _screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / _screenSize.y);

        // render current scene or the default scene
        _rt->bind();
        _rt->pushViewport(0, 0, _screenResolution.x, _screenResolution.y);
        RtAttachment dst = _rt->attachTextureAny(*_screen);
        _rt->selectAttachmentList(1, dst);

        if (_currentScene)
        {
            _currentScene->render(_rt, _screenSize, dt);
        }
        else
        {
            _defaultProg->bind();
            _defaultProg->setUniformF("mousePos", glm::vec2(relMousePos.x, relMousePos.y));
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            _defaultProg->unbind();
        }

        _rt->popViewport();
        _rt->unbind();

        // add rendered texture to ImGUI scene window
        uint64_t textureID = _screen->glName();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ (float)_screenSize.x, (float)_screenSize.y }, ImVec2{0,1}, ImVec2{1,0});
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(7);

        // add fps counter overlay
        if (_showFPSOverlay)
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

        // additional scene ui overlays
        if (_currentScene)
        {
            _currentScene->renderOverlayUI();
        }

        ImGui::End();
    }
}