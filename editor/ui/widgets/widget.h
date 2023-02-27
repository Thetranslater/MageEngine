#pragma once

#include<string>
#include<stdexcept>
#include<functional>
#include<optional>
#include<variant>
#include<memory>

#include<core/meta/reflection/reflection.h>

#include<ui/widgets/WFI/wfi.h>

#include<engine_core/function/id_allocator/id_allocator.h>

#define CREATE_WIDGET(wType, ...)	std::make_shared<wType>(__VA_ARGS__)

namespace Mage {

	template<typename T>
	class Bindable;

	const ID invalid_widget_id = INT64_MAX;

	class Widget {
	public:
		Widget() {
			ID _alloc = widget_id_allocator.alloc();
			if (isValidID(_alloc)) {
				id = _alloc;
				return;
			}
			throw std::range_error("allocating invalid id : the number of widget has reach the up limit");
		}

		virtual void draw() {}

		ID getID() const { return id; }

		void addWFI(std::shared_ptr<WFI> wfi) { wfi->setOwner(this); customized.emplace_back(wfi); }

		static IDAllocator widget_id_allocator;
	protected:
		inline void _orderPreExecuteWFI() {
			for (const auto& item : customized) {
				item->preprocess();
			}
		}

		inline void _invertPostExecuteWFI() {
			for (auto riter = customized.rbegin(); riter != customized.rbegin(); ++riter) {
				riter->operator->()->postprocess();
			}
		}

		inline bool isValidID(ID _id) {
			return _id != invalid_widget_id;
		}
		ID id;

		std::vector<std::shared_ptr<WFI>> customized;
	};

	template<typename T>
	class DataWidget : public Widget {
		friend class Bindable<T>;
	public:
		DataWidget() = default;
		DataWidget(const T& val) : value{ val } {}
	protected:
		T value{};
	};

	inline IDAllocator Widget::widget_id_allocator;
}