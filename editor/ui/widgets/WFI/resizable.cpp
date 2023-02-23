#include<imgui-docking/imgui.h>

#include"ui/widgets/WFI/resizable.h"

namespace Mage {
	void Resizable::preprocess() {
		ImGui::SetNextItemWidth(is_fixed ? width : width = width_getter());
	}

	void Resizable::postprocess() {}

}