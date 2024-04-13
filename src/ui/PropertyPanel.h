/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include <map>
#include <memory>
#include <string>
#include <functional>
#include "glm/glm.hpp"


namespace nhahn
{
    class Scene;

    class PropertyPanel
    {
        using MapOfScenes = std::map<std::string, std::shared_ptr<Scene>>;

    public:
        PropertyPanel();

        void render();
        
        void addScene(std::string name, std::shared_ptr<Scene> scene);
        void setSceneSwitchedCallback(std::function<void(std::shared_ptr<Scene>)> func);

    protected:
        MapOfScenes _scenesMap;
        std::string _currentSceneKey;

    private:
        std::function<void(std::shared_ptr<Scene>)> _sceneSwitchedCB = {};
    };
}