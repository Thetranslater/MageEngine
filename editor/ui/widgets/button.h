#pragma once

#include<core/math/vector2.h>

#include<ui/widgets/widget.h>

#include<engine_core/function/event/event.h>

namespace Mage {
	class Button : public Widget {
	public:
		Button() = default;
		Button(const std::string& l) : lable{ l } {}
		Button(float _x, float _y) :size(_x, _y) {};

		void draw() override;

		Event<> clicked_event;
	protected:
		std::string lable;
		Vector2 size;
	};
}