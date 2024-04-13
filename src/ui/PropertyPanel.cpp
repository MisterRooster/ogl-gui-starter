/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "PropertyPanel.h"

#include "imgui.h"
#include "ui/IconFontDefines.h"
#include "ui/CustomWidgets.h"

namespace nhahn
{
    void PropertyPanel::render()
    {
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos(), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(ICON_MDI_COGS " Properties"))
        {
            ImGuiStyle& style = ImGui::GetStyle();
            float w = ImGui::CalcItemWidth();
            float spacing = style.ItemInnerSpacing.x;

            ImGui::AlignTextToFramePadding();
            ImGui::NewLine();
            ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), "Tip: ");
            ImGui::TextWrapped(
			    "Remove the Demo window by uncommenting the IMGUI_DISABLE_DEMO_WINDOWS macro in "
                "the file thirdparty/oglp_imgui_config.h"
		    );
            ImGui::Spacing();
		    ImGui::NewLine();

            ImGui::SeparatorText("Example category 1:");
            ImGui::NewLine();

            ImGui::SliderFloat("Variable1", &_roughness, 0.0f, 1.0f);
            ImGui::SameLine(); ImGui::HelpMarker("CTRL+click to input value.");
            ImGui::SliderFloat("Variable2", &_metallic, 0.0f, 1.0f);
            ImGui::SameLine(); ImGui::HelpMarker("CTRL+click to input value.");

            ImGui::NewLine();
            ImGui::SeparatorText("Example category 2:");
            ImGui::NewLine();

            if (ImGui::CollapsingHeader("Collapsing Header", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::ColorEdit4("color var", (float*)&_color);
		        ImGui::SameLine(); ImGui::HelpMarker(
			        "Click on the color square to open a color picker.\n"
			        "Click and hold to use drag and drop.\n"
			        "Right-click on the color square to show options.\n"
			        "CTRL+click on individual component to input value.\n");
            }
        }
        ImGui::End();
    }
}