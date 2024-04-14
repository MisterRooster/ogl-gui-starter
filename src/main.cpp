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
#include "scene/DefaultScene.h"
#include "scene/VolumetricStanfordScene.h"
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
	std::shared_ptr<Scene> defaultScene = nullptr;
	std::shared_ptr<Scene> raymarchScene = nullptr;

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

		// render ui elements
		sceneView->render(dt);
		propertyPanel->render();
		// ImGui::ShowDemoWindow(); //<- activate for overview of imgui functionality
	}
}

//------------------------------------ application entry point -------------------------------------

int main()
{
	using namespace nhahn;

	{
		// create application with render loop
		Application app("OpenGL GUI starter project", true /* with custom titlebar */);
		app.setRenderCallback(render, renderTickrate);
		
		// create ui
		sceneView = std::make_unique<SceneView>(glm::uvec2(640, 320));
		propertyPanel = std::make_unique<PropertyPanel>();
		propertyPanel->setSceneSwitchedCallback([](std::shared_ptr<Scene> scene) {
			sceneView->setScene(scene);
		});

		// default scene
		defaultScene = SceneManager::createScene<DefaultScene>("< empty >");
		propertyPanel->addScene(defaultScene->getName(), defaultScene);

		// volumetric example scene
		raymarchScene = SceneManager::createScene<VolumetricStanfordScene>("VolumetricBunny");
		propertyPanel->addScene(raymarchScene->getName(), raymarchScene);

		// -> CREATE YOUR CUSTOM SCENES HERE

		// run main loop
		app.run();

		propertyPanel.reset();
		sceneView.reset();
	}

	exit(EXIT_SUCCESS);
}