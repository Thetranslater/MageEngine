#include<imgui-docking/imgui.h>

#include"ui/widgets/drag_float.h"

namespace Mage {
	void DragFloat::draw() {
		if (max < min) max = min;
		if (value < min) value = min;
		else if (value > max) value = max;

		ImGuiSliderFlags flags{ ImGuiSliderFlags_None };
		if (is_always_clamp)	flags |= ImGuiSliderFlags_AlwaysClamp;
		if (is_no_input)		flags |= ImGuiSliderFlags_NoInput;

		_orderExecuteWFI();

		if (ImGui::DragFloat((lable + "##" + std::to_string(id)).c_str(), &value, speed, min, max, "%.2f", flags)) {
			changed_event.invoke(value);
		}

		_invertExitWFI();
	}
}