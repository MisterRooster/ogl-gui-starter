/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "SceneView.h"

#include <imgui.h>

#include "IconFontDefines.h"
#include "ui/UIScale.h"
#include "render/BufferObject.h"
#include "scene/DefaultScene.h"
#include "utility/FileSystem.h"
#include "utility/Debug.h"
#include "utility/Utils.h"


namespace nhahn
{
    // Offscreen render-target presets selectable at runtime (see drawRenderSizeControl).
    const SceneView::RenderPreset SceneView::_renderPresets[] = {
        { "Low (960 x 540)",    glm::uvec2(960, 540)   },
        { "Medium (1280 x 720)", glm::uvec2(1280, 720)  },
        { "High (1920 x 1080)",  glm::uvec2(1920, 1080) }
    };
    const int SceneView::_renderPresetCount = (int)(sizeof(_renderPresets) / sizeof(_renderPresets[0]));

    SceneView::SceneView()
        // NB: use the constant, not _renderPresetIndex: members are initialized in
        // declaration order and _renderPresetIndex is declared after _screenResolution,
        // so it is not yet initialized at this point.
        : _screenResolution(_renderPresets[_defaultRenderPreset].resolution), _screenSize(400, 225)
    {
        // create the offscreen render target + texture we render the scene into
        createScreenTexture(_renderPresets[_renderPresetIndex].resolution);

        DBG("SceneView", DebugLevel::DEBUG, "Texture memory usage: %dmb\n", (int)(Texture::memoryUsage() / (1024 * 1024)));
    }

    void SceneView::createScreenTexture(glm::uvec2 resolution)
    {
        _screenResolution = resolution;

        // Rebuild the render target too. It caches raw pointers to its attached
        // textures, so reusing it after the old screen texture is destroyed below
        // would leave a dangling pointer (dereferenced by the next attachTextureAny()).
        _rt = std::make_shared<RenderTarget>();

        const size_t byteSize = (size_t)resolution.x * resolution.y * sizeof(float) * 4;

        // blank buffer to clear the texture to black on (re)creation
        BufferObject* blackPbo = new BufferObject(PIXEL_UNPACK_BUFFER, (GLsizei)byteSize);
        blackPbo->bind();
        blackPbo->map();
        memset(blackPbo->data(), 0, byteSize);
        blackPbo->unmap();
        blackPbo->unbind();

        // replace the screen texture (old one is released by the unique_ptr)
        _screen = std::make_unique<Texture>(TEXTURE_2D, resolution.x, resolution.y);
        _screen->setFormat(TEXEL_FLOAT, 4, 4);
        _screen->init();
        _screen->copyPbo(*blackPbo);

        delete blackPbo;

        DBG("SceneView", DebugLevel::DEBUG, "render target resized to %ux%u\n", resolution.x, resolution.y);
    }

    void SceneView::drawRenderSizeControl()
    {
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), ICON_MDI_EYE " Render Size :");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##render_size", _renderPresets[_renderPresetIndex].name))
        {
            for (int i = 0; i < _renderPresetCount; i++)
            {
                bool is_selected = (_renderPresetIndex == i);
                if (ImGui::Selectable(_renderPresets[i].name, is_selected) && i != _renderPresetIndex)
                {
                    _renderPresetIndex = i;
                    createScreenTexture(_renderPresets[i].resolution);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    SceneView::~SceneView()
    {
        // release smart pointers
        _screen.reset();
        _rt.reset();
    }

    void SceneView::render(double dt)
    {
        _currentFPS = (int)std::round(1.0 / dt);

        const float s = UIScale::instance().value();

        // scene Window
        ImGuiWindowFlags screenflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f * s, 2.0f * s));

        // set initial window size on first use
        ImGui::SetNextWindowSize(ImVec2(400.0f * s, 225.0f * s), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_MDI_EYE " Scene View", nullptr, screenflags);
        ImGui::PopStyleVar(3);

        // set screensize to viewport size
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _screenSize = { viewportPanelSize.x, viewportPanelSize.y };

        // get relative mouse position
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 scrPos = ImGui::GetCursorScreenPos();
        ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / _screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / _screenSize.y);

        // render current scene
        if (_currentScene)
        {
            _rt->bind();
            _rt->pushViewport(0, 0, _screenResolution.x, _screenResolution.y);
            RtAttachment dst = _rt->attachTextureAny(*_screen);
            _rt->selectAttachmentList(1, dst);
            _currentScene->render(_rt, _screenSize, dt);
            _rt->popViewport();
            _rt->unbind();
        }

        // add rendered texture to ImGUI scene window
        uint64_t textureID = _screen->glName();
        ImGui::Image((ImTextureID)textureID, ImVec2{ (float)_screenSize.x, (float)_screenSize.y }, ImVec2{0,1}, ImVec2{1,0});
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(7);

        // add fps counter overlay
        if (_showFPSOverlay)
        {
            char statsLabel[64];
            snprintf(statsLabel, sizeof statsLabel, ICON_MDI_SPEEDOMETER " : %i fps", _currentFPS);
            ImVec2 labelSize = ImGui::CalcTextSize(statsLabel);

            ImGuiWindowFlags statsInfo_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

            const float statsInfo_margin = 10.0f * s;
            const ImVec2 statsInfo_pad = ImVec2(10.0f * s, 4.0f * s);

            ImVec2 statsInfo_size = ImVec2(labelSize.x + statsInfo_pad.x * 2, labelSize.y + statsInfo_pad.y * 2);

            ImGui::SameLine();
            ImVec2 statsInfo_pos;
            statsInfo_pos.x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - statsInfo_margin;
            statsInfo_pos.y = ImGui::GetCursorScreenPos().y + statsInfo_margin;

            ImGui::SetNextWindowPos(statsInfo_pos, ImGuiCond_Always, ImVec2(1.0f, 0.0f));
            statsInfo_flags |= ImGuiWindowFlags_NoMove;

            ImGui::SetNextWindowBgAlpha(0.15f); // Transparent background
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f * s);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, statsInfo_pad);

            if (ImGui::BeginChild("StatsInfo", statsInfo_size, ImGuiChildFlags_Borders, statsInfo_flags))
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