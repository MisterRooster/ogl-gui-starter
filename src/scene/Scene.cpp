/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "Scene.h"

#include "render/RenderTarget.h"


namespace nhahn
{
	Scene::~Scene()
	{
		gSceneManager().unregisterScene(_name);
	}

	SceneManager& SceneManager::instance()
	{
		static SceneManager* instance = new SceneManager();
		return *instance;
	}

	SceneManager::~SceneManager()
	{
		for (auto& scene : _scenes)
		{
			scene.second->destroyScene();
		}

		_scenes.clear();
	}

	std::shared_ptr<Scene> SceneManager::getScene(std::string name)
	{
		SceneManager& gSceneManager = instance();

		if (auto search = gSceneManager._scenes.find(name);
			search != gSceneManager._scenes.end())
		{
			return gSceneManager._scenes.at(name);
		}

		return nullptr;
	}

	bool SceneManager::registerScene(std::shared_ptr<Scene> scene)
	{
		SceneManager& gSceneManager = instance();

		if (!scene) return false;
		if (auto search = gSceneManager._scenes.find(scene->getName());
			search != gSceneManager._scenes.end()) return false;

		gSceneManager._scenes.insert(std::pair<std::string, std::shared_ptr<Scene>>(scene->getName(), scene));
		return true;
	}

	bool SceneManager::unregisterScene(std::string name)
	{
		SceneManager& gSceneManager = instance();

		if (auto search = gSceneManager._scenes.find(name); search == gSceneManager._scenes.end())
		{
			search->second->destroyScene();
			gSceneManager._scenes.erase(search);
			return true;
		}

		return false;
	}

	SceneManager& gSceneManager() { return SceneManager::instance(); }
}