/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include "scene/Scene.h"
#include "render/Shader.h"
#include <glm/glm.hpp>

namespace nhahn
{
	class SquareBackgroundScene : public Scene
	{
	public:
		SquareBackgroundScene(std::string name) : Scene(name) {}
		virtual ~SquareBackgroundScene() {}

		void initializeScene() override;
		void destroyScene() override;

		void render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenResolution, double dt) override;
		void renderPropertyUI() override;

	private:
		std::unique_ptr<Shader> _squareBackgroundProg;

		double _timeSinceStart = 0;

		glm::vec3 _bgColor = glm::vec3(0.01f, 0.16f, 0.42f);
		glm::vec3 _rectColor = glm::vec3(0.01f, 0.26f, 0.57f);
		int _rectCount = 60;
	};
}
