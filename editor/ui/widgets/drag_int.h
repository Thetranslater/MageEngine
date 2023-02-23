#pragma once

#include<ui/widgets/widget.h>

#include<engine_core/event/event.h>

namespace Mage {
	class DragInt : DataWidget<int> {
	public:
		DragInt() = default;
		DragInt(const std::string& l) : lable{ l } {}

		void draw() override;
	protected:
		std::string lable;

		int min{INT_MIN};
		int max{INT_MAX};
		float speed{ 1.f };
	public:
		bool is_always_clamp{ false };
		bool is_no_input{ false };

		Event<int> changed_event;
	};
}