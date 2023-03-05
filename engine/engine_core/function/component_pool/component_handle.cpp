
#include<cassert>

#include"engine_core/function/component_pool/component_handle.h"
#include"engine_core/function/component_pool/component_pool_base.h"

namespace Mage {

	ComponentHandle::ComponentHandle() :index{ NULL_HANDLE } {}
	ComponentHandle::ComponentHandle(const ComponentHandle& copy) : index{ copy.index }, pool{ copy.pool } {}
	ComponentHandle::ComponentHandle(ComponentHandle&& move) : index{ move.index } { move.index = NULL_HANDLE; }
	ComponentHandle::ComponentHandle(int i) :index{ i } {}

	bool ComponentHandle::isValid() const { return index >= 0; }
	Component* ComponentHandle::get() const { assert(isValid()); return pool->access(index); }

	Component* ComponentHandle::operator->() const { assert(isValid()); return pool->access(index); }
	Component& ComponentHandle::operator*() const { assert(isValid());  return *pool->access(index); }

	ComponentHandle& ComponentHandle::operator=(const ComponentHandle& copy) { index = copy.index; pool = copy.pool; return *this; }
	ComponentHandle& ComponentHandle::operator=(ComponentHandle&& move) { 
		index = move.index; move.index = NULL_HANDLE; pool = move.pool; move.pool = nullptr; 
		return *this; 
	}

	bool ComponentHandle::operator==(const ComponentHandle& ComH) { return index == ComH.index && pool == ComH.pool; }

	ComponentHandle ComponentHandle::operator++(int) { assert((index + 1) >= 0); ComponentHandle ret{ index }; ++index; return ret; }
	ComponentHandle ComponentHandle::operator++() { assert((index + 1) >= 0); return ComponentHandle{ ++index }; }
	ComponentHandle ComponentHandle::operator--(int) { assert((index - 1) >= 0); ComponentHandle ret{ index }; --index; return ret; }
	ComponentHandle ComponentHandle::operator--() { assert((index - 1) >= 0); return ComponentHandle{ --index }; }
	ComponentHandle ComponentHandle::operator+(int distance) const { assert((index + distance) >= 0); return ComponentHandle{ index + distance }; }
	ComponentHandle ComponentHandle::operator-(int distance) const { assert((index - distance) >= 0); return ComponentHandle{ index - distance }; }

	ComponentHandle::operator Mage::Reflection::ReflectionPtr<Mage::Component>() const {
		Component* comp_ptr = pool->access(*this);
		return Reflection::ReflectionPtr<Component>{typeid(comp_ptr).name(), comp_ptr};
	}

	ComponentHandle operator+(int distance, const ComponentHandle& cHandle) { return cHandle + distance; }
}