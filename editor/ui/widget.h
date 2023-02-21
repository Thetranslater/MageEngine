#pragma once

#include<stdexcept>
#include<memory>

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
		inline ID getID() const { return id; }

		static IDAllocator widget_id_allocator;
	protected:
		inline bool isValidID(ID _id) {
			return _id != invalid_widget_id;
		}
		ID id;
	};

	inline IDAllocator Widget::widget_id_allocator;
}