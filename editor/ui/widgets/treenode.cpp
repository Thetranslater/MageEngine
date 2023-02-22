#include<imgui-docking/imgui.h>

#include"ui/widgets/treenode.h"

namespace Mage {
	void TreeNode::draw() {
		ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_None };

		if (is_leaf)					flags |= ImGuiTreeNodeFlags_Leaf;
		if (is_default_open)			flags |= ImGuiTreeNodeFlags_DefaultOpen;
		if (is_open_on_arrow)			flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		if (is_open_on_double_click)	flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (is_selected)				flags |= ImGuiTreeNodeFlags_Selected;
		if (is_span_full_width) 		flags |= ImGuiTreeNodeFlags_SpanFullWidth;

		bool open = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, label.c_str());

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			clicked_event.invoke();
		}

		if (open) {
			for (const auto& node : widgets) {
				node->draw();
			}

			ImGui::TreePop();
		}
	}
}