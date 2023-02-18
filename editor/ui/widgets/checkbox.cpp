#include<imgui-docking/imgui.h>

#include"ui/widgets/checkbox.h"

namespace Mage {
	void CheckBox::draw() {
		if (ImGui::Checkbox(label_id.c_str(), &value)) {
			changed_event.invoke(value);
		}
	}
}