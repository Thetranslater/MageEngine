#pragma once

#include<imgui-docking/imgui.h>

#include<ui/widget.h>

namespace Mage {
	class SameLine : public Widget {
	public:
		SameLine() = default;
		SameLine(float off, float s) : offset{ off }, spacing{ s } {}

		void draw() override {
			ImGui::SameLine(offset, spacing);
		}

	private:
		float offset{ 0.f };
		float spacing{ -1.f };
	};
}