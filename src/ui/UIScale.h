/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include "imgui.h"


namespace nhahn
{
	/**
	 * Global UI/DPI scale manager.
	 *
	 * Drives a single "UI scale" factor that scales both the ImGui fonts (dynamically
	 * re-rasterized and thus crisp, via style.FontScaleMain in ImGui 1.92+) and the
	 * style metrics (padding/spacing/rounding/etc via ImGuiStyle::ScaleAllSizes).
	 *
	 * The scale is initialized from the monitor content scale at startup (e.g. 1.5 for
	 * 150% Windows scaling) and can be adjusted at runtime through drawControl().
	 * Widget/chrome code that still uses hardcoded pixel literals (e.g. the custom
	 * titlebar) should multiply them by value().
	 */
	class UIScale
	{
	public:
		static UIScale& instance();

		/** Capture the current (unscaled) ImGui style as the reference and apply the
			auto-detected content scale. Call once after the style + fonts are set up. */
		void init(float dpiScale);

		/** Current effective UI scale (1.0 = 100% = 96 DPI). */
		float value() const { return _scale; }

		/** Content scale detected from the monitor at startup (the "Auto" default). */
		float autoScale() const { return _dpiScale; }

		/** Clamp, store and re-apply the scale to the ImGui style (no-op if unchanged). */
		void setScale(float scale);

		/** Renders the "UI Scale" slider (+ reset-to-auto button). Returns true on change. */
		bool drawControl();

	private:
		UIScale() = default;

		/** Rebuild the live ImGui style from the captured base at the current scale. */
		void apply();

	private:
		float _scale = 1.0f;
		float _dpiScale = 1.0f;      // auto-detected monitor content scale (slider default)
		ImGuiStyle _baseStyle;       // unscaled reference style captured at init()
		bool _initialized = false;
	};
}
