/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "SceneView.h"

#include "imgui.h"
#include "render/BufferObject.h"
#include "utility/FileSystem.h"
#include "utility/Debug.h"


namespace nhahn
{
    SceneView::SceneView(std::shared_ptr<Texture> t) : _srcD(t), _screenSize(400, 225)
    {
        _srcSize = glm::vec2(t->width(), t->height());

        //create new render target
        _rt = std::make_unique<RenderTarget>();

        std::string path = nhahn::FileSystem::getModuleDirectory() + "shaders/";
        _quad = std::make_unique<Shader>(path.c_str(), "common.inc", "quad.vert", nullptr, "quad.frag", 1);

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

        DBG("UI", DebugLevel::DEBUG, "Texture memory usage: %dmb\n", (int)(Texture::memoryUsage() / (1024 * 1024)));
    }

    SceneView::~SceneView()
    {
        _screen.reset();
        _quad.reset();
        _rt.reset();
    }

    void SceneView::render()
    {
        // scene Window
        ImGuiWindowFlags screenflags = ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNavFocus;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos(), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2((float)_screenSize.x, (float)_screenSize.y), ImGuiCond_FirstUseEver);
        ImGui::Begin("Scene view", nullptr, screenflags);
        ImGui::PopStyleVar(3);

        // get relative mouse position
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 scrPos = ImGui::GetCursorScreenPos();
        ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x)/_screenSize.x,1.0f-(io.MousePos.y - scrPos.y)/_screenSize.y);

        // Render source textures to screen texture
        _rt->bind();
        _rt->pushViewport(0, 0, _srcSize.x, _srcSize.y);
        _srcD->bindAny();
        RtAttachment dst = _rt->attachTextureAny(*_screen);
        _rt->selectAttachmentList(1, dst);
        _quad->bind();
        _quad->setUniformI("D", _srcD->boundUnit());
        _quad->setUniformF("mousePos", glm::vec2(relMousePos.x, relMousePos.y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _rt->popViewport();
        _rt->unbind();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _screenSize = { viewportPanelSize.x, viewportPanelSize.y };

        // add rendered texture to ImGUI scene window
        uint64_t textureID = _screen->glName();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ (float)_screenSize.x, (float)_screenSize.y }, ImVec2{0,1}, ImVec2{1,0});
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(7);
        {
            // add description hint to corner
            ImVec2 newPos;
            const char* btnLabel = "Render example";
            newPos.x = ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(btnLabel).x - 18.f;
            ImGui::SameLine();
            newPos.y = ImGui::GetCursorPosY() + 10.f;
            ImGui::SetCursorPos(newPos);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.6f, 0.6f, 0.35f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.35f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.6f, 0.6f, 0.35f));
            ImGui::Button(btnLabel);
            ImGui::PopStyleColor(3);
        }
        ImGui::End();
    }
}