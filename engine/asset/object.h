#pragma once
#include<vector>
#include<string>

#include"core/meta/reflection/reflection.h"

namespace Mage {
	class Component;

	REFLECTION_TYPE(ObjectAsset)
	CLASS(ObjectAsset, Fields) {
		REFLECTION_BODY(ObjectAsset)
	public:
		std::string name;
		std::vector<Reflection::ReflectionPtr<Component>> components;
	};
}