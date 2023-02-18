#pragma once

#include<imgui-docking/imgui.h>

#include<ui/widget.h>
#include<ui/container.h>

namespace Mage {
	class Group : public Widget, public Container{
	public:
		Group() = default;
		template<typename W, typename ...WidgetT>
		Group(W&& w, WidgetT&&... ws) : Group(ws...) {
			W& ref_w = w;
			widgets.emplace_back(std::addressof(ref_w));
		}

		void draw() override {
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