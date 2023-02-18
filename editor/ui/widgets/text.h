#pragma once

#include<imgui-docking/imgui.h>

#include<ui/widget.h>

namespace Mage {
	class Text : public Widget {
	public:
		Text(const std::string& l) : lable{ l } {}

		inline void draw() override {
			ImGui::Text(lable.c_str());
		}

		template<typename ...ty>
		void draw(ty... args) {
			ImGui::Text(lable.c_str(), args...);
		}
	protected:
		std::string lable;
	};
}