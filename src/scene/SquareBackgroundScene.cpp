/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "SquareBackgroundScene.h"

#include "ui/IconFontDefines.h"
#include "ui/CustomWidgets.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
    void SquareBackgroundScene::initializeScene()
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _squareBackgroundProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "squareBackground.frag", 1);
        _timeSinceStart = 0.0;
    }

    void SquareBackgroundScene::destroyScene()
    {
        _squareBackgroundProg.reset();
    }

    void SquareBackgroundScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
    {
        _timeSinceStart += dt;

        // get relative mouse position, the glow follows the cursor
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 scrPos = ImGui::GetCursorScreenPos();
        ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / screenSize.y);

        _squareBackgroundProg->bind();
        _squareBackgroundProg->setUniformF("mousePos", glm::vec2(relMousePos.x, relMousePos.y));
        _squareBackgroundProg->setUniformF("resolution", glm::vec3(screenSize.x, screenSize.y, 1.0f));
        _squareBackgroundProg->setUniformF("time", (float)_timeSinceStart);
        _squareBackgroundProg->setUniformF("bgColor", _bgColor);
        _squareBackgroundProg->setUniformF("rectColor", _rectColor);
        _squareBackgroundProg->setUniformF("total", (float)_rectCount);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _squareBackgroundProg->unbind();
    }

    void SquareBackgroundScene::renderPropertyUI()
    {
        ImGui::SeparatorText("Description:");
        ImGui::TextWrapped(
            "A calm blue background loop. Rotating squares of random size drift across a "
            "cloudy gradient that is built from animated fbm noise. The glow in the "
            "background follows the mouse cursor.\n\n"
            "This shader is converted from Shadertoy.com. All credits belong to the original author."
        );
        ImGui::NewLine();
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Shadertoy link : ");
        ImGui::SameLine(); ImGui::Text("https://www.shadertoy.com/view/MdVXzw");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Author : ");
        ImGui::SameLine(); ImGui::Text("vamoss");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Date : ");
        ImGui::SameLine(); ImGui::Text("2016-05-18");
        ImGui::NewLine();

        ImGui::SeparatorText("Settings:");
        ImGui::ColorEdit3("Background Color", (float*)&_bgColor);
        ImGui::ColorEdit3("Rect Color", (float*)&_rectColor);
        ImGui::SliderInt("Number of Rectangles", &_rectCount, 0, 100);
    }
}
