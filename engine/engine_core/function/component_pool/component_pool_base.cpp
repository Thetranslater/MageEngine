#include"engine_core/function/component_pool/component_pool_base.h"
#include"engine_core/function/component_pool/component_handle.h"

namespace Mage {
	ComponentPoolBase::ComponentPoolBase() : _size{ 0 }, _capacity{ 0 } {}

	ComponentHandle ComponentPoolBase::begin() {
		ComponentHandle h{ 0 };
		h.pool = this;
		return h;
	}

	ComponentHandle ComponentPoolBase::end() {
		ComponentHandle h{ _size };
		h.pool = this;
		return h;
	}

	const ComponentHandle ComponentPoolBase::cbegin() {
		ComponentHandle h{ 0 };
		h.pool = this;
		return h;
	}

	const ComponentHandle ComponentPoolBase::cend() {
		ComponentHandle h{ _size };
		h.pool = this;
		return h;
	}
}