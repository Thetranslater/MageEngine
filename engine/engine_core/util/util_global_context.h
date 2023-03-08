#pragma once

#include<string>
#include<unordered_map>

#include<engine_core/function/component_pool/component_pool.h>

namespace Mage {
	namespace Util {
		extern std::unordered_map<std::string, ComponentPoolBase*> component_pools;

		ComponentHandler MakeComponent(const std::string& type);
	}
}