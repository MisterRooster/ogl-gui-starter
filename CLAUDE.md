# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

`ogl-gui-starter` is a small OpenGL application template built on GLFW3 with Dear ImGui (docking branch) as the GUI layer. It is meant as a starting point for prototyping OpenGL scenarios: you add a **Scene**, and the app gives you a docked property panel, an offscreen-rendered scene viewport, input handling, and helper classes for shaders/textures/framebuffers/file IO. Requires an **OpenGL 4.4 core** capable GPU. Primary target is Windows (Visual Studio 2022); MinGW/GCC builds are also supported.

## Build & Run

Dependencies (`glfw`, `glew-cmake`, `glm`, `imgui`) are git submodules under `dependencies/`. CMake auto-runs `git submodule update --init --recursive` during configure, so a plain clone builds — but `git clone --recursive` (or `git submodule update --init --recursive`) avoids a first-configure failure.

```bash
# Configure (Visual Studio 2022, multi-config generator)
cmake -B build -G "Visual Studio 17 2022"

# Build
cmake --build build --config Debug     # or --config Release
```

- The executable is written to `bin/` (multi-config generators use `bin/Debug/` or `bin/Release/`).
- A POST_BUILD step copies `data/` and `src/shaders/` next to the executable (shaders land in `data/shaders/`). At runtime the app loads assets relative to the executable via `FileSystem::getModuleDirectory()`, **not** the source tree — so shaders/fonts/icons must be rebuilt (or the copy re-run) after editing to take effect.
- **Debug vs Release differ in more than optimization:** Debug links the CONSOLE subsystem and enables the `DBG`/`ASSERT`/`FAIL` logging macros; Release links the WINDOWS subsystem (no console) and compiles those macros out (`NDEBUG`). Use Debug when you need log output.

There is no test suite for this project (the `dependencies/*` folders contain the upstream libraries' own tests, which are disabled in this build).

## Commit

Do not add a co-author text to the commits.

## Architecture

All application code lives under `src/` in `namespace nhahn`.

**Main loop.** `main.cpp` constructs an `Application`, which owns the GLFW `Window`. `Application::run()` drives a loop that fires a single **render callback** at a fixed tickrate (~60 fps) and any number of **update callbacks**, each with its own tickrate (input polling is registered as one). `Window::preRender`/`postRender` bracket each render callback and delegate to two render contexts (see `render/RenderContext.h`):
- `GLContext` — clears/swaps the OpenGL buffers.
- `UIContext` — drives the ImGui frame and, when enabled, the custom titlebar (window dragging/minimize/maximize).

**Scenes are the extension point.** A `Scene` (see `scene/Scene.h`) is an abstract class with `initializeScene` / `destroyScene` / `render(rt, screenSize, dt)` plus optional `renderPropertyUI()` and `renderOverlayUI()`. `SceneManager` is a singleton registry keyed by name; `SceneManager::createScene<T>(name)` instantiates the scene, calls `initializeScene`, and registers it.

**UI wiring.** `main.cpp::render()` calls `SceneView::render()` and `PropertyPanel::render()` each frame:
- `PropertyPanel` (`ui/PropertyPanel.h`) lists registered scenes; selecting one invokes a callback (wired in `main.cpp`) into `SceneView::setScene`, and it renders the active scene's `renderPropertyUI()`.
- `SceneView` (`ui/SceneView.cpp`) renders the active scene **into an offscreen `RenderTarget`/`Texture` (render-to-texture)** and then draws that texture as an `ImGui::Image` inside the docked "Scene View" window, plus an FPS overlay.
- The initial docking layout (Properties left, Scene View center) is built once in `main.cpp::splitDockspace()` and thereafter persisted in `imgui.ini` next to the executable.

**Supporting layers:**
- `render/` — `Shader`/`ShaderObject` (multi-source shaders with `refresh()` hot-reload and typed `setUniform*` helpers), `RenderTarget` (FBO), `Texture`, `BufferObject` (PBO/VBO), `VertexBuffer`.
- `input/` — `Input` singleton (keyboard/mouse via GLFW callbacks) and `Gamepad` (XInput on Windows), polled through an Application update callback.
- `utility/` — `Debug` (logging/assert macros), `FileSystem` (file IO + `getModuleDirectory()`), `Timer`, `Utils`, `Types`.
- `thirdparty/` — `ImGuiBuild.cpp` compiles ImGui and its GLFW/OpenGL3 backends into the target; `oglp_imgui_config.h` is the `IMGUI_USER_CONFIG` (set via CMake); `stb_image` for image loading.

## Adding a new scene (the primary workflow)

1. Create `src/scene/MyScene.{h,cpp}` subclassing `Scene` (use `scene/SimpleCubeScene.*` as the reference implementation). Constructor forwards the name: `MyScene(std::string name) : Scene(name) {}`.
2. Implement `initializeScene` (build VAOs/VBOs, load shaders from `FileSystem::getModuleDirectory() + "data\\shaders\\"`), `destroyScene`, `render`, and optionally `renderPropertyUI`.
3. Register it in `main.cpp` at the `// -> CREATE YOUR CUSTOM SCENES HERE` marker:
   ```cpp
   auto myScene = SceneManager::createScene<MyScene>("MyScene");
   propertyPanel->addScene(myScene->getName(), myScene);
   ```

New source files are picked up automatically by the CMake `GLOB_RECURSE` over `src/`, but **re-run CMake configure** after adding files so the glob re-evaluates.

## Conventions

- Every source file starts with the project license banner comment block ("This notice is not to be removed") — preserve it on new files.
- Globally-accessed singletons are exposed either as `Xxx::instance()` (`Application`, `SceneManager`, `Input`) or via a `gXxx()` free function (`gInput()`, `gSceneManager()`).
- Private members are `_camelCase` (some older code in `Input` uses `m_` — match the surrounding file).
- ImGui window/label strings use Material Design Icon macros from `ui/IconFontDefines.h` (e.g. `ICON_MDI_EYE " Scene View"`). Reusable ImGui helpers (e.g. `HelpMarker`) live in `ui/CustomWidgets`.
- Log with the `DBG("Module", DebugLevel::LEVEL, "fmt\n", ...)` macro; it is a no-op in Release, so never rely on it for control flow.
