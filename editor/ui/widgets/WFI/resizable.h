#pragma once

#include<functional>

#include<ui/widgets/WFI/wfi.h>

namespace Mage {
	//Order requirement: Resizable must preprocess at the end.
	class Resizable : public WFI {
	public:
		Resizable() = default;
		Resizable(float w) : width{ w }, is_fixed{ true } {}

		void preprocess() override;
		void postprocess() override;

		void setFixedWidth(float w) { width = w; is_fixed = true; }
	protected:
		std::function<float()> width_getter;
		float width{ 0.f };
		//TODO:Reference<float> width;
		bool is_fixed{ false };
	};
}