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

		if (hasBind()) value = get();
		//TODO:怎么抽象组件的布局方法呢？
		ImGui::SetNextItemWidth(60.f);
		if (ImGui::DragFloat(lable_id.c_str(), &value, speed, min, max, "%.2f", flags)) {
			changed_event.invoke(value);
		}

		if (hasBind()) set(value);
	}
}