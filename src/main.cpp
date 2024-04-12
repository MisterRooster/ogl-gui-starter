/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/

#include "imgui.h"
#include "Application.h"
#include "ui/SceneView.h"
#include "ui/PropertyPanel.h"
#include "utility/Debug.h"

//--------------------------------------------------------------------------------------------------

namespace nhahn
{
	static int renderFPS = 0;
	std::unique_ptr<SceneView> sceneView;
	std::unique_ptr<PropertyPanel> propertyPanel;
	std::shared_ptr<Texture> displayTex;

	void render(double dt)
	{
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
		Application app("GUI start project");
		app.addUpdateCallback([](double dt) { DBG("Application", DebugLevel::DEBUG, "Fps: %d\n", renderFPS); }, 5.0);
		app.setRenderCallback(render, 1.0 / 60.0);

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