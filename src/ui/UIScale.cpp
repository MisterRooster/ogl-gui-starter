/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022-2026 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#include "UIScale.h"

#include <algorithm>

#include "ui/IconFontDefines.h"
#include "utility/Debug.h"


namespace nhahn
{
	static const float kMinScale = 0.5f;
	static const float kMaxScale = 3.0f;

	UIScale& UIScale::instance()
	{
		static UIScale s;
		return s;
	}

	void UIScale::init(float dpiScale)
	{
		_dpiScale = (dpiScale > 0.0f) ? dpiScale : 1.0f;
		// capture the current, still-unscaled style as our reference
		_baseStyle = ImGui::GetStyle();
		_scale = std::clamp(_dpiScale, kMinScale, kMaxScale);
		_initialized = true;
		apply();
		DBG("UIScale", DebugLevel::DEBUG, "initialized UI scale to %.2f from monitor content scale\n", _scale);
	}

	void UIScale::setScale(float scale)
	{
		scale = std::clamp(scale, kMinScale, kMaxScale);
		if (scale == _scale) return;
		_scale = scale;
		if (_initialized) apply();
	}

	void UIScale::apply()
	{
		// Start from the unscaled reference so repeated applies never compound.
		ImGuiStyle scaled = _baseStyle;
		scaled.ScaleAllSizes(_scale);       // padding / spacing / rounding / borders
		scaled.FontScaleMain = _scale;      // fonts (dynamically re-rasterized -> crisp)
		ImGui::GetStyle() = scaled;
	}

	bool UIScale::drawControl()
	{
		bool changed = false;
		float s = _scale;

		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(ImVec4(1.0f, 0.628f, 0.311f, 1.0f), ICON_MDI_ARROW_LEFT_RIGHT " UI Scale :");
		ImGui::SameLine();

		// leave room on the right for the reset button
		float resetW = ImGui::GetFrameHeight();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - resetW - ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::SliderFloat("##ui_scale", &s, kMinScale, kMaxScale, "%.2fx"))
		{
			setScale(s);
			changed = true;
		}

		ImGui::SameLine();
		if (ImGui::Button(ICON_MDI_WINDOW_RESTORE "##ui_scale_reset", ImVec2(resetW, resetW)))
		{
			setScale(_dpiScale);
			changed = true;
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Reset to detected display scale (%.2fx)", _dpiScale);

		return changed;
	}
}
