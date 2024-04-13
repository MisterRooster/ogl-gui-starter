/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "render/Shader.h"
#include "render/Texture.h"
#include "render/RenderTarget.h"


namespace nhahn
{
    class Texture;
    class Shader;
    class RenderTarget;
    class Scene;

    class SceneView
    {
    public:
        SceneView(glm::uvec2 resolution);
        ~SceneView();

        void render(double dt);

        void setScene(std::shared_ptr<Scene> scene) { _currentScene = scene; }
        
        void showFPSOverlay(bool fps) { _showFPSOverlay = fps; }

    private:
        std::unique_ptr<Shader> _defaultProg;
        std::shared_ptr<RenderTarget> _rt;

        std::unique_ptr<Texture> _screen;
        glm::uvec2 _screenResolution;
        glm::uvec2 _screenSize;

        std::shared_ptr<Scene> _currentScene = nullptr;

        int _currentFPS = 0;
        bool _showFPSOverlay = true;
    };
}