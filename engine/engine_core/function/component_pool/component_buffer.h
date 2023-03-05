#pragma once

#define MAX_COMPONENT_CONTAIN 32

namespace Mage {
	template<typename Ty>
	struct ComponentBuffer {
		Ty components[MAX_COMPONENT_CONTAIN];
	};
}