#pragma once
#include<string>
#include<vector>

#include"core/meta/reflection/reflection.h"

namespace Mage {
	REFLECTION_TYPE(WorldAsset)
	CLASS(WorldAsset,Fields) {
		REFLECTION_BODY(WorldAsset)
	public:
		std::string name;
		int default_scene;
		std::vector<std::string> scene_urls;
	};
}