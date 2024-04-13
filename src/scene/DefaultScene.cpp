/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "DefaultScene.h"

#include "ui/IconFontDefines.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
	void DefaultScene::initializeScene()
	{
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _defaultProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "default.frag", 1);
	}

	void DefaultScene::destroyScene()
	{
        _defaultProg.reset();
	}

	void DefaultScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
	{
		// get relative mouse position
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 scrPos = ImGui::GetCursorScreenPos();
		ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / screenSize.y);

        _defaultProg->bind();
        _defaultProg->setUniformF("mousePos", glm::vec2(relMousePos.x, relMousePos.y));
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _defaultProg->unbind();
	}

	void DefaultScene::renderPropertyUI()
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		ImGui::TextWrapped(
			ICON_MDI_HELP_CIRCLE " This Property window is dockable, "
			"hold the SHIFT key to dock the window while dragging it around"
		);
		ImGui::PopStyleColor();
	}
}