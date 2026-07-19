/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "GildedPlumesScene.h"

#include "ui/IconFontDefines.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
    void GildedPlumesScene::initializeScene()
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _gildedPlumesProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "gildedPlumes.frag", 1);
        _timeSinceStart = 0.0;
    }

    void GildedPlumesScene::destroyScene()
    {
        _gildedPlumesProg.reset();
    }

    void GildedPlumesScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
    {
        _timeSinceStart += dt;

        _gildedPlumesProg->bind();
        _gildedPlumesProg->setUniformF("resolution", glm::vec3(screenSize.x, screenSize.y, 1.0f));
        _gildedPlumesProg->setUniformF("time", (float)_timeSinceStart);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _gildedPlumesProg->unbind();
    }

    void GildedPlumesScene::renderPropertyUI()
    {
        ImGui::SeparatorText("Description:");
        ImGui::TextWrapped(
            "Golden fractal plumes spiral through deep teal folds, revealing intricate "
            "recursive patterns in a slow rotating zoom. \n\n"
            "This shader is converted from Shadertoy.com. All credits belong to the original author."
        );
        ImGui::NewLine();
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Shadertoy link : ");
        ImGui::SameLine(); ImGui::Text("https://www.shadertoy.com/view/7fc3WH");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Author : ");
        ImGui::SameLine(); ImGui::Text("FabriceNeyret2");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Date : ");
        ImGui::SameLine(); ImGui::Text("2024-06-01");
    }
}
