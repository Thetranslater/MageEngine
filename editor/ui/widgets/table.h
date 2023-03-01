#pragma once

#include<array>

#include<imgui-docking/imgui.h>

#include<ui/widgets/widget.h>
#include<ui/widgets/container.h>

namespace Mage {
	template<uint32_t r, uint32_t c>
	class Table : public Widget {
	public:
		Table() = default;

		void draw() override {
			ImGuiTableFlags flags = ImGuiTableFlags_None;

			if (is_size_fixed_fit) flags |= ImGuiTableFlags_SizingFixedFit;
			if (is_resizable) flags |= ImGuiTableFlags_Resizable;

			if (ImGui::BeginTable(("##" + std::to_string(id)).c_str(), c, flags)) {

				for (int row{ 0 }; row < r; ++row) {
					ImGui::TableNextRow();
					for (int col{ 0 }; col < c; ++col) {
						ImGui::TableSetColumnIndex(col);
						widgets[row][col]->draw();
					}
				}

				ImGui::EndTable();
			}
		}

		std::array<std::shared_ptr<Widget>, c>& operator[](int index) {
			return widgets[index];
		}

	protected:
		std::array<std::array<std::shared_ptr<Widget>, c>, r> widgets;

	public:
		bool is_size_fixed_fit{ false };
		bool is_resizable{ false };
	};
}