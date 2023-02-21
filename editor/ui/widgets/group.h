#pragma once

#include<imgui-docking/imgui.h>

#include<ui/widget.h>
#include<ui/container.h>

namespace Mage {
	class Group : public Widget, public Container{
	public:
		Group() = default;
		template<typename W, typename ...WidgetT>
		Group(W&& w, WidgetT&&... ws) : Group{ws...} {
			widgets.emplace_back(w);
		}

		void draw() override {
			ImGui::Indent(10.f);
			ImGui::BeginGroup();

			for (const auto& widget : widgets) {
				widget->draw();
			}

			is_hoverd = ImGui::IsItemHovered();

			ImGui::EndGroup();
		}
	public:
		bool is_hoverd{ false };
	};
}