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
#include "render/VertexBuffer.h"
#include <glm/glm.hpp>

namespace nhahn
{
	class SimpleCubeScene : public Scene
	{
	public:
		SimpleCubeScene(std::string name) : Scene(name) {}
		virtual ~SimpleCubeScene() {}

		void initializeScene() override;
		void destroyScene() override;

		void render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenResolution, double dt) override;
		void renderPropertyUI() override;

	private:
		std::unique_ptr<Shader> _phongShadingProg;
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ibo = 0;

		double _timeSinceStart = 0;

		bool _transparentBackground = true;
		glm::vec3 _lightPos = glm::vec3(-1.2f, 1.0f, 2.0f);
		glm::vec3 _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	};
}