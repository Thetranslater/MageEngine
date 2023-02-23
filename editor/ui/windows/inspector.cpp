#include"core/meta/reflection/reflection.h"

#include"ui/windows/inspector.h"
#include"ui/widgets/sameline.h"
#include"ui/widgets/seperator.h"
#include"ui/widgets/group.h"
#include"ui/widgets/checkbox.h"
#include"ui/widgets/input_text.h"
#include"ui/widgets/treenode.h"
#include"ui/editor_ui.h"

//#include"engine_core/scaffold/components/component.h"
#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"
#include"engine_core/scaffold/game_object.h"

namespace Mage {
	Inspector::Inspector(const std::string& id) :Window(id) {}

	Inspector::Inspector(const std::string& id, const WindowConfig& config) : Window(id, config) {}

	//TODO:components ui£¬ °ó¶¨£¬²¼¾Ö
	void Inspector::OnSelected(GameObject& selected) {
		widgets.clear();

		//header
		auto iText = CREATE_WIDGET(InputText);
		iText->setContent(selected.name());
		auto header = CREATE_WIDGET(Group, iText, CREATE_WIDGET(SameLine, 0.f, 10.f), CREATE_WIDGET(CheckBox, true));

		widgets.emplace_back(header);

		//seperator
		widgets.emplace_back(CREATE_WIDGET(Seperator));

		//components
		auto component_ptr = selected.GetComponent(TransformComponent);

		auto node_ptr = EditorUI::base_widget_creator["TreeNode"]("TransformComponent", (void*)component_ptr);
		widgets.emplace_back(node_ptr);

	}

	void Inspector::draw() {
		ImGuiWindowFlags flags = _convert_config_to_imgui();

		if (!ImGui::Begin(window_id.c_str(), configuration.no_close ? nullptr : &is_close, flags)) {
			ImGui::End();
			return;
		}

		_update();

		for (const auto& widget : widgets) {
			widget->draw();
		}

		ImGui::End();
	}
}