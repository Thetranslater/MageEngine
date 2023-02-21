#include"ui/windows/inspector.h"
#include"ui/widgets/sameline.h"
#include"ui/widgets/group.h"
#include"ui/widgets/checkbox.h"
#include"ui/widgets/input_text.h"

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
		std::shared_ptr<Group> header = std::make_shared<Group>(iText, CREATE_WIDGET(SameLine, 0.f, 10.f), CREATE_WIDGET(CheckBox, true));

		widgets.emplace_back(header);

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