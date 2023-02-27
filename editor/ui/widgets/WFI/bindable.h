#pragma once

#include<functional>
#include<optional>

#include<ui/widgets/widget.h>
#include<ui/widgets/WFI/wfi.h>

namespace Mage {
	//Order requirement: Binable must preprocessed at the first and postprocessed in the last;
	template<typename dataTy>
	class Bindable : public WFI {
		using getter = std::function<dataTy(void)>;
		using setter = std::function<void(dataTy&)>;
	public:
		Bindable() = default;
		Bindable(const getter& get, const setter& set) : accessor{ std::make_pair(get,set) } {}

		void bind(const getter& get, const setter& set) { accessor = std::make_pair(get, set); }

		bool hasBind() { return accessor.has_value(); }

		void preprocess() override { static_cast<DataWidget<dataTy>*>(parent)->value = accessor.value().first(); }
		void postprocess() override { accessor.value().second(static_cast<DataWidget<dataTy>*>(parent)->value); }
	protected:
		std::optional<std::pair<getter, setter>> accessor;
	};
}