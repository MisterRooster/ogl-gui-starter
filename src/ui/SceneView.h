/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "render/Texture.h"
#include "render/RenderTarget.h"


namespace nhahn
{
    class Texture;
    class RenderTarget;
    class Scene;
    class DefaultScene;

    class SceneView
    {
    public:
        SceneView();
        ~SceneView();

        void render(double dt);

        void setScene(std::shared_ptr<Scene> scene) { _currentScene = scene; }

        void showFPSOverlay(bool fps) { _showFPSOverlay = fps; }

        /** Combobox to pick the offscreen render resolution (Low/Medium/High).
            Rebuilds the render-target texture when the selection changes. */
        void drawRenderSizeControl();

        struct RenderPreset { const char* name; glm::uvec2 resolution; };

    private:
        /** (Re)create the offscreen render-target texture at the given resolution. */
        void createScreenTexture(glm::uvec2 resolution);

    private:
        std::shared_ptr<RenderTarget> _rt;

        std::unique_ptr<Texture> _screen;
        glm::uvec2 _screenResolution;
        glm::uvec2 _screenSize;

        std::shared_ptr<Scene> _currentScene = nullptr;

        static const RenderPreset _renderPresets[];
        static const int _renderPresetCount;
        static constexpr int _defaultRenderPreset = 1; // Medium
        int _renderPresetIndex = _defaultRenderPreset;

        int _currentFPS = 0;
        bool _showFPSOverlay = true;
    };
}