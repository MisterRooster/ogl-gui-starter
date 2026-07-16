/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "EndlessCreatureScene.h"

#include "ui/IconFontDefines.h"
#include "ui/CustomWidgets.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
    void EndlessCreatureScene::initializeScene()
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _endlessCreatureProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "endlessCreature.frag", 1);
        _timeSinceStart = 0.0;
    }

    void EndlessCreatureScene::destroyScene()
    {
        _endlessCreatureProg.reset();
    }

    void EndlessCreatureScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
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

        _endlessCreatureProg->bind();
        _endlessCreatureProg->setUniformF("mousePos", mouseUnif);
        _endlessCreatureProg->setUniformF("resolution", glm::vec3(screenSize.x, screenSize.y, 1.0f));
        _endlessCreatureProg->setUniformF("time", (float)_timeSinceStart);
        _endlessCreatureProg->setUniformI("count", _count);
        _endlessCreatureProg->setUniformF("speed", _speed);
        _endlessCreatureProg->setUniformF("balance", _balance);
        _endlessCreatureProg->setUniformF("range", _range);
        _endlessCreatureProg->setUniformF("radius", _radius);
        _endlessCreatureProg->setUniformF("blend", _blend);
        _endlessCreatureProg->setUniformF("falloff", _falloff);
        _endlessCreatureProg->setUniformF("motionFrames", (float)_motionFrames);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _endlessCreatureProg->unbind();
    }

    void EndlessCreatureScene::renderOverlayUI()
    {
        Scene::renderOverlayUI();

        // add input info
        {
            const char* inputlabel1 = "[" ICON_MDI_MOUSE "LMB + " ICON_MDI_ARROW_ALL "] " ICON_MDI_ARROW_RIGHT_THIN " Turn Camera";
            ImVec2 labelSize1 = ImGui::CalcTextSize(inputlabel1);

            ImGuiWindowFlags inputInfo_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

            const float inputInfo_margin = 10.0f;
            const ImVec2 inputInfo_pad = ImVec2(10.0f, 4.0f);

            ImVec2 inputInfo_size = ImVec2(labelSize1.x + inputInfo_pad.x * 2,
                labelSize1.y  + inputInfo_pad.y * 2);

            ImVec2 inputInfo_pos;
            inputInfo_pos.x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - inputInfo_margin;
            inputInfo_pos.y = ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - inputInfo_margin;

            ImGui::SetNextWindowPos(inputInfo_pos, ImGuiCond_Always, ImVec2(1.0f, 1.0f));
            inputInfo_flags |= ImGuiWindowFlags_NoMove;

            ImGui::SetNextWindowBgAlpha(0.15f); // Transparent background

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, inputInfo_pad);
            if (ImGui::BeginChild("InputInfo", inputInfo_size, ImGuiChildFlags_Borders, inputInfo_flags))
            {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), inputlabel1);
            }
            ImGui::PopStyleVar(2);
            ImGui::EndChild();
        }
    }

    void EndlessCreatureScene::renderPropertyUI()
    {
        ImGui::SeparatorText("Description:");
        ImGui::TextWrapped(
            "A strange creature that never ends. A sphere is mirrored and rotated over and "
            "over at shrinking scales, which grows a body of limbs that keeps crawling past "
            "the camera. Drag with the left mouse button to turn the view.\n\n"
            "This shader is converted from Shadertoy.com. All credits belong to the original author."
        );
        ImGui::NewLine();
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Shadertoy link : ");
        ImGui::SameLine(); ImGui::Text("https://www.shadertoy.com/view/tljXWy");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Author : ");
        ImGui::SameLine(); ImGui::Text("Leon Denise (ponk)");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Date : ");
        ImGui::SameLine(); ImGui::Text("2019-08-28");
        ImGui::NewLine();

        ImGui::SeparatorText("Settings:");
        ImGui::SliderInt("Motion Frames", &_motionFrames, 1, 5);
        ImGui::SameLine();
        ImGui::HelpMarker(
            "Number of motion blur samples per pixel.\n"
            "The whole scene is raymarched once per sample, so every step "
            "up costs you roughly one more full render. Increase at your own GPU risk."
        );
        ImGui::NewLine();

        if (ImGui::CollapsingHeader("Tweak Zone", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderInt("Count", &_count, 1, 30);
            ImGui::SameLine();
            ImGui::HelpMarker("Number of fold iterations. Each one mirrors and rotates the\n"
                "space again, so more of them grow a more detailed body.");

            ImGui::SliderFloat("Speed", &_speed, 0.0f, 5.0f);
            ImGui::SameLine();
            ImGui::HelpMarker("Animation speed of the creature. Set to 0 to freeze it.");

            ImGui::SliderFloat("Balance", &_balance, 0.0f, 5.0f);
            ImGui::SameLine();
            ImGui::HelpMarker("How far the limbs rotate on every fold.");

            ImGui::SliderFloat("Range", &_range, 0.0f, 3.0f);
            ImGui::SameLine();
            ImGui::HelpMarker("Spacing of the mirrored copies, spreads the limbs apart.");

            ImGui::SliderFloat("Radius", &_radius, 0.05f, 2.0f);
            ImGui::SameLine();
            ImGui::HelpMarker("Radius of the sphere each limb is built from.");

            ImGui::SliderFloat("Blend", &_blend, 0.01f, 1.0f);
            ImGui::SameLine();
            ImGui::HelpMarker("Smoothing radius used to melt the spheres into each other.");

            ImGui::SliderFloat("Falloff", &_falloff, 1.0f, 2.5f);
            ImGui::SameLine();
            ImGui::HelpMarker("How quickly the limbs shrink on every fold.\n"
                "At 1.0 they all keep the same size.");
        }
    }
}
