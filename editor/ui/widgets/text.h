#pragma once

#include<imgui-docking/imgui.h>

#include<ui/widgets/widget.h>

namespace Mage {
	class Text : public Widget {
	public:
		Text(const std::string& l) : content{ l } {}

		inline void draw() override {
			ImGui::Text(content.c_str());
		}

		template<typename ...ty>
		void draw(ty... args) {
			ImGui::Text(format.c_str(), args...);
		}
	protected:
		std::string content;
		std::string& format{ content };
	};
}