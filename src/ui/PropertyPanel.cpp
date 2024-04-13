/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "PropertyPanel.h"

#include "imgui.h"
#include "scene/Scene.h"
#include "ui/IconFontDefines.h"
#include "ui/CustomWidgets.h"
#include "utility/Debug.h";


namespace nhahn
{
    PropertyPanel::PropertyPanel()
    {
        _scenesMap = MapOfScenes{};
        _currentSceneKey = "";
    }

    void PropertyPanel::addScene(std::string name, std::shared_ptr<Scene> scene)
    {
        if (!scene) return;
        if (auto search = _scenesMap.find(name); search != _scenesMap.end()) return;

        _scenesMap[name] = scene;

        if (_currentSceneKey.empty())
        {
            _currentSceneKey = name;
            if (_sceneSwitchedCB) _sceneSwitchedCB(scene);
        }
    }

    void PropertyPanel::setSceneSwitchedCallback(std::function<void(std::shared_ptr<Scene>)> func)
    {
        if (func) _sceneSwitchedCB = func;
        DBG("PropertyPanel", DebugLevel::DEBUG, "changed onSceneSwitched callback\n");
    }

    void PropertyPanel::render()
    {
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos(), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(ICON_MDI_COGS " Properties"))
        {
            ImGuiStyle& style = ImGui::GetStyle();
            float w = ImGui::CalcItemWidth();
            float spacing = style.ItemInnerSpacing.x;

            ImGui::SeparatorText("Scene:");

            ImGui::AlignTextToFramePadding();
            ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), ICON_MDI_SHIMMER " Current Scene :");

            ImGui::SameLine();
            ImGui::PushItemWidth(w - spacing * 2.0f);
            if (ImGui::BeginCombo("##cb_scene", _currentSceneKey.c_str()))
            {
                for (auto const& [key, val] : _scenesMap)
                {
                    bool is_selected = (_currentSceneKey == key);
                    if (ImGui::Selectable(key.c_str(), is_selected))
                    {
                        _currentSceneKey = key;
                        if (_sceneSwitchedCB) _sceneSwitchedCB(_scenesMap[_currentSceneKey]);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
            ImGui::NewLine();

            if (!_currentSceneKey.empty())
                _scenesMap[_currentSceneKey]->renderPropertyUI();
        }
        ImGui::End();
    }
}