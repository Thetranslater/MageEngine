#include<imgui-docking/imgui.h>

#include"ui/widgets/checkbox.h"

namespace Mage {
	void CheckBox::draw() {
		_orderPreExecuteWFI();

		if (ImGui::Checkbox((lable + "##" + std::to_string(id)).c_str(), &value)) {
			changed_event.invoke(value);
		}

		_invertPostExecuteWFI();
	}
}