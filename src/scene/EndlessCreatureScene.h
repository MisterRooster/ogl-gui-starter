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
#include <glm/glm.hpp>

namespace nhahn
{
	class EndlessCreatureScene : public Scene
	{
	public:
		EndlessCreatureScene(std::string name) : Scene(name) {}
		virtual ~EndlessCreatureScene() {}

		void initializeScene() override;
		void destroyScene() override;

		void render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenResolution, double dt) override;
		void renderOverlayUI() override;
		void renderPropertyUI() override;

	private:
		std::unique_ptr<Shader> _endlessCreatureProg;

		double _timeSinceStart = 0;
		glm::vec2 mouseUnif = glm::vec2(0);

		// tweak zone
		int _count = 15;
		float _speed = 1.0f;
		float _balance = 1.5f;
		float _range = 1.4f;
		float _radius = 0.6f;
		float _blend = 0.3f;
		float _falloff = 1.2f;
		int _motionFrames = 1;
	};
}
