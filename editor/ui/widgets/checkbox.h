#pragma once
#include<string>

#include<ui/widgets/widget.h>
#include<ui/widgets/WFI/bindable.h>

#include<engine_core/event/event.h>

namespace Mage {
	class CheckBox : public DataWidget<bool> {
	public:
		CheckBox() = default;
		CheckBox(bool v) : DataWidget{ v } {}
		CheckBox(bool v, const std::string& l) : DataWidget{ v }, lable{ l } {}
		CheckBox(const std::string& l) : lable{ l } {}

		void draw() override;
	protected:
		std::string lable;
	public:
		Event<bool> changed_event;
	};
}