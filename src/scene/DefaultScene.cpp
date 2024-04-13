/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "DefaultScene.h"

#include "ui/IconFontDefines.h"
#include "render/Shader.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include <imgui.h>


namespace nhahn
{
	void DefaultScene::initializeScene()
	{
        std::string path = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";

        _raymarchSimpleDFProg = std::make_unique<Shader>(path.c_str(), "common.inc", "fullScreenQuad.vert", nullptr, "raymarchDF.frag", 1);
	}

	void DefaultScene::destroyScene()
	{
        _raymarchSimpleDFProg.reset();
	}

	void DefaultScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
	{
		// get relative mouse position
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 scrPos = ImGui::GetCursorScreenPos();
		ImVec2 relMousePos = ImVec2((io.MousePos.x - scrPos.x) / screenSize.x, 1.0f - (io.MousePos.y - scrPos.y) / screenSize.y);

		_raymarchSimpleDFProg->bind();
        //_raymarchSimpleDFProg->setUniformMat("modelViewMat", _cam->getViewMatrix(), false);
        //_raymarchSimpleDFProg->setUniformMat("projectionMat", projMat, false);

		_raymarchSimpleDFProg->setUniformF("mousePos", glm::vec2(relMousePos.x, relMousePos.y));
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		_raymarchSimpleDFProg->unbind();
	}

	void DefaultScene::renderOverlayUI()
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

	void DefaultScene::renderPropertyUI()
	{
        Scene::renderPropertyUI();
	}
}