/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "SimpleCubeScene.h"

#include "ui/IconFontDefines.h"
#include "render/RenderTarget.h"
#include "utility/FileSystem.h"
#include "ui/CustomWidgets.h"
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace nhahn
{
	void SimpleCubeScene::initializeScene()
	{
		std::string shaderPath = nhahn::FileSystem::getModuleDirectory() + "data\\shaders\\";
		_phongShadingProg = std::make_unique<Shader>(shaderPath.c_str(), "common.inc", "basicMVP.vert", nullptr, "phongShading.frag", 1);

		float vertices[] = {
			 // front
			 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

			 // back
			-1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f, 1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,-1.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,

			 // right
			 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			 // left
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

			 // top
			 1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

			 // bottom
			 1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};
		unsigned int indices[] = 
		{
			0,1,2, 0,2,3,
			4,5,6, 4,6,7,
			8,9,10, 8,10,11,
			12,13,14, 12,14,15,
			16,17,18, 16,18,19,
			20,21,22, 20,22,23
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// vertex buffer
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		GLint attrIdx = glGetAttribLocation(_phongShadingProg->id(), "aPos");
		glVertexAttribPointer(attrIdx, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const GLvoid*)0);
		glEnableVertexAttribArray(attrIdx);
		attrIdx = glGetAttribLocation(_phongShadingProg->id(), "aNormal");
		glVertexAttribPointer(attrIdx, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(attrIdx);
		attrIdx = glGetAttribLocation(_phongShadingProg->id(), "aColor");
		glVertexAttribPointer(attrIdx, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const GLvoid*)(6 * sizeof(float)));
		glEnableVertexAttribArray(attrIdx);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		 
		// index buffer
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		_timeSinceStart = 0.0;
	}

	void SimpleCubeScene::destroyScene()
	{
		_phongShadingProg.reset();
	}

	void SimpleCubeScene::render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenSize, double dt)
	{
		_timeSinceStart += dt;
		 
		glm::mat4 model = glm::rotate(glm::scale(glm::mat4(1.0f),glm::vec3(0.5f)), (float)_timeSinceStart * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenSize.x / (float)screenSize.y, 0.1f, 100.0f);

		_phongShadingProg->bind();
		_phongShadingProg->setUniformMat("model", model, false);
		_phongShadingProg->setUniformMat("view", view, false);
		_phongShadingProg->setUniformMat("projection", projection, false);
		_phongShadingProg->setUniformF("lightPos", _lightPos);
		_phongShadingProg->setUniformF("lightColor", _lightColor);
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, _transparentBackground ? 0.0f : 1.0f);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		_phongShadingProg->unbind();
	}

	void SimpleCubeScene::renderPropertyUI()
	{
		ImGui::SeparatorText("Description:");
		ImGui::TextWrapped( 
			"This scene renders a simple cube with the phong shading model "
			"and transforms the vertices with model, "
			"view and projection matrices."
		);
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Author : ");
		ImGui::SameLine(); ImGui::Text("MisterRooster");
		ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Date : ");
		ImGui::SameLine(); ImGui::Text("2022-10-21");
		ImGui::NewLine();

		ImGui::SeparatorText("Settings:");
		ImGui::NewLine();
		ImGui::Checkbox("Transparent Background", &_transparentBackground);
		ImGui::NewLine();

		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat3("Light Position", (float*)&_lightPos, -2.0f, 2.0f);
			ImGui::ColorEdit3("Light Color", (float*)&_lightColor);
			ImGui::SameLine();
			ImGui::HelpMarker(
				"Click on the color square to open a color picker.\n"
				"Click and hold to use drag and drop.\n"
				"Right-click on the color square to show options.\n"
				"CTRL+click on individual component to input value.\n"
			);
		}
	}
}