
#include<cassert>

#include"engine_core/scaffold/components/component.h"
#include"engine_core/function/component_pool/component_handle.h"

namespace Mage {
	ComponentHandler::ComponentHandler() = default;
	ComponentHandler::ComponentHandler(int i, ComponentPoolBase* p) : index{ i }, pool{ p } {}
	ComponentHandler::operator Reflection::ReflectionPtr<Component>() const {
		Component* instance{ pool->handleAccess(this) };
		return Reflection::ReflectionPtr<Component>{instance->getTypeName(), instance};
	}
	ComponentHandler::operator Component* () const {
		return pool->handleAccess(this);
	}
	Component* ComponentHandler::operator->() const {
		return static_cast<Component*>(pool->handleAccess(this));
	}
	Component& ComponentHandler::operator*() const {
		return *static_cast<Component*>(pool->handleAccess(this));
	}
	bool ComponentHandler::operator==(const ComponentHandler& r) const { return r.index == index and r.pool == pool; }
	ComponentHandler ComponentHandler::operator++(int) { ComponentHandler ret{ index, pool }; ++index; return ret; }
	ComponentHandler& ComponentHandler::operator++() { ++index; return *this; }
	ComponentHandler ComponentHandler::operator--(int) { ComponentHandler ret{ index, pool }; --index; return ret; }
	ComponentHandler& ComponentHandler::operator--() { --index; return *this; }
	ComponentHandler ComponentHandler::operator+(int distance) const {
		assert((index + distance) >= 0);
		return ComponentHandler{ index + distance, pool };
	}
	ComponentHandler ComponentHandler::operator-(int distance) const {
		assert((index - distance) >= 0);
		return ComponentHandler{ index - distance, pool };
	}
	ComponentHandler operator+(int distance, const ComponentHandler& handle) {
		return handle + distance;
	}
}