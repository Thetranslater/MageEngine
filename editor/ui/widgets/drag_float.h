#pragma once

#include<ui/widgets/widget.h>

#include<engine_core/event/event.h>

namespace Mage {
	class DragFloat : public LableIDWidget, public Bindable<float>{
	public:
		DragFloat() = default;
		DragFloat(float mi, float ma, float spd = 0.1f) :min{ mi }, max{ ma }, speed{ spd } {}

		void draw() override;
	protected:
		float value;

		float min{FLT_MIN};
		float max{FLT_MAX};
		float speed{0.1f};
	public:
		bool is_always_clamp{ false };
		bool is_no_input{ false };

		Event<float> changed_event;
	};
}