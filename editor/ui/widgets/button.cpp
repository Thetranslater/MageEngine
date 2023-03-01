#include<imgui-docking/imgui.h>

#include"ui/widgets/button.h"

namespace Mage {
	void Button::draw() {
		_orderExecuteWFI();

		if (ImGui::Button((lable + "##" + std::to_string(id)).c_str(), ImVec2{size.x, size.y})) {
			clicked_event.invoke();
		}

		_invertExitWFI();
	}
}