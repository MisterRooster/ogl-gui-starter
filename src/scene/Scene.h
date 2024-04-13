/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include "utility/Debug.h"
#include <string>
#include <memory>
#include <unordered_map>
#include "glm/glm.hpp"


namespace nhahn
{
	class SceneManager;
	class RenderTarget;

	class Scene
	{
	public:
		Scene(std::string name);
		virtual ~Scene();

		std::string getName() { return _name; }

		virtual void initializeScene() = 0;
		virtual void destroyScene() = 0;

		virtual void render(std::shared_ptr<RenderTarget>& rt, glm::uvec2 screenResolution, double dt) = 0;
		virtual void renderOverlayUI() {}
		virtual void renderPropertyUI() {}

	protected:
		std::string _name = "BaseScene";
	};

	class SceneManager
	{
	public:
		static SceneManager& instance();
		virtual ~SceneManager();

		static std::shared_ptr<Scene> getScene(std::string name);

		template<typename T>
		static std::shared_ptr<Scene> createScene(std::string name);

		static bool registerScene(std::shared_ptr<Scene> scene);
		static bool unregisterScene(std::string name);

	protected:
		friend class Scene;

		SceneManager() {}

	private:
		std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
	};

	template<typename T>
	static std::shared_ptr<Scene> SceneManager::createScene(std::string name)
	{
		SceneManager& gSceneManager = instance();

		if (auto search = gSceneManager._scenes.find(name);
			search != gSceneManager._scenes.end())
		{
			DBG("SceneManager", DebugLevel::WARNING, "Scene with name %s already exists!\n");
			return nullptr;
		}

		std::shared_ptr<Scene> scene = std::make_shared<T>(name);
		scene->initializeScene();
		gSceneManager._scenes.insert(std::pair<std::string, std::shared_ptr<Scene>>(name, scene));

		return scene;
	}

	SceneManager& gSceneManager();
}