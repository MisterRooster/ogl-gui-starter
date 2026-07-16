/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "LightningStormScene.h"

#include "ui/IconFontDefines.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
    void LightningStormScene::initializeScene()
    {
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _lightningStormProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "lightningStorm.frag", 1);
        _timeSinceStart = 0.0;
    }

    void LightningStormScene::destroyScene()
    {
        _lightningStormProg.reset();
    }

    void LightningStormScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
    {
        _timeSinceStart += dt;

        _lightningStormProg->bind();
        _lightningStormProg->setUniformF("resolution", glm::vec3(screenSize.x, screenSize.y, 1.0f));
        _lightningStormProg->setUniformF("time", (float)_timeSinceStart);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _lightningStormProg->unbind();
    }

    void LightningStormScene::renderPropertyUI()
    {
        ImGui::SeparatorText("Description:");
        ImGui::TextWrapped(
            "Flickering bolts of light in a swirling storm. The image is built by folding the "
            "screen coordinates over and over, so every pixel is found without any geometry.\n\n"
            "This shader is converted from Shadertoy.com. All credits belong to the original author."
        );
        ImGui::NewLine();
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Shadertoy link : ");
        ImGui::SameLine(); ImGui::Text("https://www.shadertoy.com/view/XXyGzh");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Author : ");
        ImGui::SameLine(); ImGui::Text("SnoopethDuckDuck");
        ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Date : ");
        ImGui::SameLine(); ImGui::Text("2024-06-01");
    }
}
