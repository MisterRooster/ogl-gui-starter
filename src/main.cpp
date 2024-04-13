/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/

#include "imgui.h"
#include "imgui_internal.h"
#include "Application.h"
#include "ui/SceneView.h"
#include "ui/PropertyPanel.h"
#include "ui/IconFontDefines.h"
#include "utility/Debug.h"
#include "utility/FileSystem.h"
#include "utility/Utils.h"

//--------------------------------------------------------------------------------------------------

namespace nhahn
{
	const double renderTickrate = 1.0 / 60.0;

	std::unique_ptr<SceneView> sceneView;
	std::unique_ptr<PropertyPanel> propertyPanel;
	std::shared_ptr<Texture> displayTex;

	void splitDockspace()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::Begin("MainDockspaceWindow");

			ImGuiID dockSpaceId = ImGui::GetID("MainDockspace");
			ImGuiID dockspace_id_copy = dockSpaceId;

			ImGui::DockBuilderRemoveNode(dockSpaceId); // clear any previous layout
			ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetMainViewport()->Size);

			auto dock_id_left = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, 0.25f, nullptr, &dockSpaceId);
			ImGui::DockBuilderDockWindow(ICON_MDI_COGS " Properties", dock_id_left);
			ImGui::DockBuilderDockWindow(ICON_MDI_EYE " Scene View", dockSpaceId);
			ImGui::DockBuilderFinish(dockSpaceId);
			DBG("UI", DebugLevel::DEBUG, "docked child windows to main dockspace\n");

			ImGui::End();
		}
	}

	void render(double dt)
	{
		// split dockspace and dock subwindows on programs first run
		std::string path = nhahn::FileSystem::getModuleDirectory() + "imgui.ini";
		static auto firstCall = true;
		bool firstTimeEver = firstCall && !FileSystem::fileExists(path.c_str());
		if (firstTimeEver) {
			firstCall = false;
			splitDockspace();
		}

		// calculate frames per second
		renderFPS = (int)(1.0 / dt);

		// render ui elements
		sceneView->render();
		propertyPanel->render();
		ImGui::ShowDemoWindow();
	}
}

//------------------------------------ application entry point -------------------------------------

int main()
{
	using namespace nhahn;

	{
		Application app("OpenGL GUI starter project", true /* with custom titlebar */);
		app.setRenderCallback(render, renderTickrate);

		displayTex = std::make_shared<Texture>(TextureType::TEXTURE_2D, 640, 320);
		sceneView = std::make_unique<SceneView>(displayTex);
		propertyPanel = std::make_unique<PropertyPanel>();

		// run main loop
		app.run();

		propertyPanel.reset();
		sceneView.reset();
		displayTex.reset();
	}

	exit(EXIT_SUCCESS);
}