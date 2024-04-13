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

    class SceneView
    {
    public:
        SceneView(std::shared_ptr<Texture> t);
        ~SceneView();

        void render(double dt);

    private:
        int _currentFPS = 0;
        std::unique_ptr<Shader> _quadProg;
        std::unique_ptr<RenderTarget> _rt;

        std::shared_ptr<Texture> _srcD;
        std::unique_ptr<Texture> _screen;
        glm::uvec2 _srcSize;
        glm::uvec2 _screenSize;
    };
}