#pragma once

#define CREATE_WFI(wfi_Ty, ...) std::make_shared<wfi_Ty>(__VA_ARGS__)

namespace Mage {
	class Widget;
	//widget functional interface
	class WFI {
	public:
		void setOwner(Widget* owner) { parent = owner; }
		virtual void preprocess() {};
		virtual void postprocess() {};
	protected:
		Widget* parent{ nullptr };
	};
}