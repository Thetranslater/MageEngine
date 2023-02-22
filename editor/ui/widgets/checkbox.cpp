#include<imgui-docking/imgui.h>

#include"ui/widgets/checkbox.h"

namespace Mage {
	void CheckBox::draw() {
		if (hasBind()) value = get();

		if (ImGui::Checkbox(lable_id.c_str(), &value)) {
			changed_event.invoke(value);
		}

		if (hasBind()) set(value);
	}
}