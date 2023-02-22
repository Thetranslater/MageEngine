#pragma once
#include<string>

#include<ui/widgets/widget.h>

#include<engine_core/event/event.h>

namespace Mage {
	class CheckBox : public LableIDWidget, public Bindable<bool> {
	public:
		CheckBox(bool v) : value{ v } {}
		CheckBox(bool v, const std::string& l) : value{ v } {}

		inline bool Value() { return value; }

		void draw() override;
	protected:
		bool value;
	public:
		Event<bool> changed_event;
	};
}