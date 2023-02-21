#pragma once
#include<string>

#include<ui/widget.h>

#include<engine_core/event/event.h>

namespace Mage {
	class CheckBox : public Widget {
	public:
		CheckBox(bool v) : value{ v }, lable_id{"##" + std::to_string(id)} {}
		CheckBox(bool v, const std::string& l) : value{ v }, lable_id{ l + "##" + std::to_string(id)} {}

		inline bool Value() { return value; }

		void draw() override;
	protected:
		bool value;
		std::string lable_id;
	public:
		Event<bool> changed_event;
	};
}