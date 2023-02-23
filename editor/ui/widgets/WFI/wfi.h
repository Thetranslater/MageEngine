#pragma once

#define CREATE_WFI(wfi_Ty, ...) std::shared_ptr<wfi_Ty>(__VA_ARGS__)

namespace Mage {
	class Widget;
	//widget functional interface
	class WFI {
	public:
		virtual void preprocess() {};
		virtual void postprocess() {};
	protected:
		Widget* parent{ nullptr };
	};
}