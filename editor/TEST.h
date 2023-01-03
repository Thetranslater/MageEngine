#pragma once
#include"core/meta/reflection/reflection.h"
namespace Mage {

	REFLECTION_TYPE(World)
	CLASS(World, WhiteListFields) {
		REFLECTION_BODY(World)
	public:
		META(Enable)
		std::string name;
		META(Enable)
		std::vector<std::string> urls;
	};
}