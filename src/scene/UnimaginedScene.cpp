/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "UnimaginedScene.h"

#include "ui/IconFontDefines.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
    void UnimaginedScene::initializeScene()
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _unimaginedProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "unimagined.frag", 1);
        _timeSinceStart = 0.0;
    }

    void UnimaginedScene::destroyScene()
    {
        _unimaginedProg.reset();
    }

    void UnimaginedScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
    {
        _timeSinceStart += dt;

        _unimaginedProg->bind();
        _unimaginedProg->setUniformF("resolution", glm::vec3(screenSize.x, screenSize.y, 1.0f));
        _unimaginedProg->setUniformF("time", (float)_timeSinceStart);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _unimaginedProg->unbind();
    }

    void UnimaginedScene::renderPropertyUI()
    {
        ImGui::SeparatorText("Description:");
        ImGui::TextWrapped(
            "An endless flight through a repeating grid of glowing shapes. Each step of the "
            "raymarch blends a sphere and a line, so the shapes melt into each other while "
            "the view twists and moves forward.\n\n"
            "This shader is converted from Shadertoy.com. All credits belong to the original author."
        );
        ImGui::NewLine();
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Shadertoy link : ");
        ImGui::SameLine(); ImGui::Text("https://www.shadertoy.com/view/NflSD8");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Author : ");
        ImGui::SameLine(); ImGui::Text("OldEclipse");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Date : ");
        ImGui::SameLine(); ImGui::Text("2026-04-04");
    }
}
