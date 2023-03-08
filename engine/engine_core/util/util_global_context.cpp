#include"engine_core/util/util_global_context.h"

namespace Mage {
	namespace Util {
		std::unordered_map<std::string, ComponentPoolBase*> component_pools;

		ComponentHandler MakeComponent(const std::string& type) {
			return component_pools[type]->allocate();
		}
	}
}