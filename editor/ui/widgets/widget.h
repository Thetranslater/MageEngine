#pragma once

#include<string>
#include<stdexcept>
#include<functional>
#include<optional>
#include<variant>
#include<memory>

#include<core/meta/reflection/reflection.h>
#include<engine_core/function/id_allocator/id_allocator.h>

#define CREATE_WIDGET(wType, ...)	std::make_shared<wType>(__VA_ARGS__)

namespace Mage {

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

		static IDAllocator widget_id_allocator;
	protected:
		inline bool isValidID(ID _id) {
			return _id != invalid_widget_id;
		}
		ID id;
	};

	template<typename dataTy>
	class Bindable {
		using getter = std::function<dataTy(void)>;
		using setter = std::function<void(dataTy&)>;
	public:
		Bindable() = default;
		Bindable(const getter& get, const setter& set) : accessor{ std::make_pair(get,set) } {}

		constexpr void bind(const getter& get, const setter& set) { accessor = std::make_pair(get, set); }

		constexpr bool hasBind() { return accessor.has_value(); }
		
		constexpr dataTy get() { return accessor.value().first(); }
		constexpr void set(dataTy& value) { accessor.value().second(value); }
	protected:
		std::optional<std::pair<getter, setter>> accessor;
	};

	class LableIDWidget : public Widget {
	public:
		LableIDWidget() : lable_id{ "##" + std::to_string(id) } {}
		LableIDWidget(const std::string& lable) : lable_id{ lable + "##" + std::to_string(id) } {}
	protected:
		std::string lable_id;
	};

	inline IDAllocator Widget::widget_id_allocator;
}