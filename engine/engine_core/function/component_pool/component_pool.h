#pragma once
#include<vector>

#include<engine_core/function/component_pool/component_pool_base.h>
#include<engine_core/function/component_pool/component_handle.h>
#include<engine_core/scaffold/components/component.h>
#include<engine_core/scaffold/game_object.h>

namespace Mage {
	template<typename Ty>
	class ComponentPool : public ComponentPoolBase{
		using ComponentType = Ty;
	public:
		~ComponentPool() override {
			for (const auto& ptr : buffer_ptr) {
				delete ptr;
			}
		}

		ComponentHandle allocate(int num) override {
			ComponentHandle h{ _size };
			while (num > 0) {
				if (_size >= _capacity) {
					ComponentBuffer<Ty>* block = new ComponentBuffer<Ty>;
					buffer_ptr->emplace_back(block);
					_capacity += MAX_COMPONENT_CONTAIN;
					if (_capacity < 0) {
						throw std::bad_alloc{ "no more space to allocate component" };
					}
					continue;
				}
				void* place = access(_size);
				place = new (place)Ty;
				++_size;
				--num;
			}
			h.pool = this;
			return h;
		}

		bool release(ComponentHandle& handle) override {
			if (handle.pool != this or handle.index >= _size) {
				return false;
			}
			Component* tail = access(_size - 1);
			auto& handles = tail->GetGameObject()->GetComponents();
			for (const auto& h : handles) {
				if (h.pool == this and h.index == _size - 1) {
					h.index = handle.index;
					break;
				}
			}
			*(access(handle)) = *tail;
			--_size;
			return true;
		}
		Component* access(const ComponentHandle& handle) override {
			assert(handle.isValid() and handle.index < _size);
			int buffer_offset = handle.index / MAX_COMPONENT_CONTAIN;
			int location_offset = handle.index % MAX_COMPONENT_CONTAIN;
			return std::addressof(buffer_ptr[buffer_offset]->components[location_offset]);
		}
	private:
		std::vector<ComponentBuffer<Ty>*> buffer_ptr;
	};
}