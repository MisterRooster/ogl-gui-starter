/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include "scene/Scene.h"
#include "render/Shader.h"


namespace nhahn
{
	class DefaultScene : public Scene
	{
	public:
		DefaultScene(std::string name) : Scene(name) {}
		virtual ~DefaultScene() {}

		void initializeScene() override;
		void destroyScene() override;

		void render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenResolution, double dt) override;
		void renderPropertyUI() override;

	private:
		std::unique_ptr<Shader> _defaultProg;
	};
}