#pragma once

#include<imgui-docking/imgui.h>

#include<ui/widgets/widget.h>

namespace Mage {
	class Seperator :public Widget {
	public:
		Seperator() = default;

		void draw() override {
			ImGui::Separator();
		}
	};
}