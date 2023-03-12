#pragma once

#include<cassert>
#include<stdexcept>

#include<core/meta/reflection/reflection.h>

#include<engine_core/function/component_pool/component_buffer.h>
#include<engine_core/function/component_pool/component_pool_base.h>
#include<engine_core/function/component_pool/component_handle.h>
#include<engine_core/scaffold/components/component.h>
#include<engine_core/scaffold/game_object.h>

namespace Mage {

	//ComponentPool define
	template<typename Ty , typename = std::enable_if_t<std::is_convertible_v<Ty*, Component*>>>
	class ComponentPool : public ComponentPoolBase {
	public:
		ComponentHandle<Ty> begin() {
			return ComponentHandle<Ty>{0, this};
		}
		ComponentHandle<Ty> end() {
			return ComponentHandle<Ty>{_size, this};
		}

		ComponentHandler allocate(int num = 1) override {
			ComponentHandler head{ _size, this };
			while (num > 0) {
				if (_size >= _capacity) {
					ComponentBuffer<Ty>* buffer = new ComponentBuffer<Ty>;
					buffer_ptr.emplace_back(buffer);
					_capacity += MAX_COMPONENT_CONTAIN;
					if (_capacity < 0) {
						throw std::bad_alloc{};
					}
					continue;
				}
				void* place = addressOf(_size);
				place = new (place)Ty;
				++_size;
				--num;
			}
			return head;
		}

		bool release(ComponentHandle<Ty>& handle) {
			if (handle.index >= 0 and handle.index < _size and handle.pool == this) {
				Component* tail = &(operator[](_size - 1));
				tail->GetGameObject()->setDirty();
				auto& attached_components = tail->GetGameObject()->GetComponents();
				for (auto& comp : attached_components) {
					if (comp->getTypeName() == Ty::getClassName()) {
						comp.index = handle.index;
						break;
					}
				}
				operator[](handle.index) = *(static_cast<Ty*>(tail));
				--_size;
				return true;
			}
			return false;
		}

		Ty& operator[](int index) {
#if _DEBUG
			assert(index >= 0 and index < _size);
#endif
			return buffer_ptr[index / MAX_COMPONENT_CONTAIN]->components[index % MAX_COMPONENT_CONTAIN];
		}
	protected:
		Component* handleAccess(const void* h) override {
			const ComponentHandle<Ty>* handle = static_cast<const ComponentHandle<Ty>*>(h);
#if _DEBUG
			assert(handle->index >= 0 and handle->index < _size);
#endif
			return std::addressof(buffer_ptr[handle->index / MAX_COMPONENT_CONTAIN]->components[handle->index % MAX_COMPONENT_CONTAIN]);
		}
		inline Ty* addressOf(int index) { return std::addressof(buffer_ptr[index / MAX_COMPONENT_CONTAIN]->components[index % MAX_COMPONENT_CONTAIN]); }
	private:
		std::vector<ComponentBuffer<Ty>*> buffer_ptr;
	};
}