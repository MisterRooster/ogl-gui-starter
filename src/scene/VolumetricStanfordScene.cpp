/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "VolumetricStanfordScene.h"

#include "ui/IconFontDefines.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
    void VolumetricStanfordScene::initializeScene()
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _volumetricStanfordProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "volumetricStanford.frag", 1);
        _timeSinceStart = 0.0;
    }

    void VolumetricStanfordScene::destroyScene()
    {
        _volumetricStanfordProg.reset();
    }

    void VolumetricStanfordScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
    {
        _timeSinceStart += dt; 
        
        // get relative mouse position
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 scrPos = ImGui::GetCursorScreenPos(); 
        ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / screenSize.y);
        if (io.MouseDown[0] && abs(relMousePos.x-.5)<.5 && abs(relMousePos.y-.5)<.5)
        {
            mouseUnif = glm::vec2(relMousePos.x, relMousePos.y);
        }
        
        _volumetricStanfordProg->bind();
        _volumetricStanfordProg->setUniformF("mousePos", mouseUnif);
        _volumetricStanfordProg->setUniformF("resolution", glm::vec3(screenSize.x, screenSize.y, 1.0f));
        _volumetricStanfordProg->setUniformF("time", (float)_timeSinceStart);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _volumetricStanfordProg->unbind();
    }

    void VolumetricStanfordScene::renderOverlayUI()
    {
        Scene::renderOverlayUI();

        // add input info
        {
            const char* inputlabel1 = "[" ICON_MDI_MOUSE "LMB + " ICON_MDI_ARROW_ALL "] " ICON_MDI_ARROW_RIGHT_THIN " Turn Camera";
            const char* inputlabel2 = "[" ICON_MDI_MOUSE "RMB + " ICON_MDI_ARROW_UP_DOWN "] " ICON_MDI_ARROW_RIGHT_THIN " Zoom in / out";
            ImVec2 labelSize1 = ImGui::CalcTextSize(inputlabel1);
            ImVec2 labelSize2 = ImGui::CalcTextSize(inputlabel2);

            ImGuiWindowFlags inputInfo_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

            const float inputInfo_margin = 10.0f;
            const ImVec2 inputInfo_pad = ImVec2(10.0f, 4.0f);

            ImVec2 inputInfo_size = ImVec2(std::max(labelSize1.x, labelSize2.x) + inputInfo_pad.x * 2,
                labelSize1.y + labelSize2.y + inputInfo_pad.y * 2 + 9.0f);

            ImVec2 inputInfo_pos;
            inputInfo_pos.x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - inputInfo_margin;
            inputInfo_pos.y = ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - inputInfo_margin;

            ImGui::SetNextWindowPos(inputInfo_pos, ImGuiCond_Always, ImVec2(1.0f, 1.0f));
            inputInfo_flags |= ImGuiWindowFlags_NoMove;

            ImGui::SetNextWindowBgAlpha(0.15f); // Transparent background

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, inputInfo_pad);

            if (ImGui::BeginChild("InputInfo", inputInfo_size, true, inputInfo_flags))
            {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), inputlabel1);
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.06f, 0.06f, 0.06f, 1.0f));
                ImGui::Separator();
                ImGui::PopStyleColor(1);
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), inputlabel2);
            }
            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }
    }

    void VolumetricStanfordScene::renderPropertyUI()
    {
        Scene::renderPropertyUI();
    }
}