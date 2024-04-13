/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "PropertyPanel.h"

#include "imgui.h"


namespace nhahn
{
    void PropertyPanel::render()
    {
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos(), ImGuiCond_FirstUseEver);
        ImGui::Begin("Properties");
        if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Test Text");
        }

        if (ImGui::CollapsingHeader("Example Material"))
        {
            ImGui::ColorPicker3("Color", (float*)&_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
            ImGui::SliderFloat("Roughness", &_roughness, 0.0f, 1.0f);
            ImGui::SliderFloat("Metallic", &_metallic, 0.0f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Example Light"))
        {

            ImGui::Separator();
            ImGui::Text("Position");
            ImGui::Separator();
        }

        ImGui::End();
    }
}