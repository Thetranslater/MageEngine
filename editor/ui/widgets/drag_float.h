#pragma once

#include<ui/widgets/widget.h>
#include<ui/widgets/WFI/bindable.h>

#include<engine_core/function/event/event.h>

namespace Mage {
	class DragFloat : public DataWidget<float> {
	public:
		DragFloat() = default;
		DragFloat(const std::string& l) : lable{ l } {}
		DragFloat(float mi, float ma, float spd = 0.01f) :min{ mi }, max{ ma }, speed{ spd } {}

		void draw() override;
	protected:
		std::string lable;

		float min{-FLT_MAX};
		float max{FLT_MAX};
		float speed{0.01f};
	public:
		bool is_always_clamp{ false };
		bool is_no_input{ false };

		Event<float> changed_event;
	};
}