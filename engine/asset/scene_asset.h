#pragma once
#include<string>

#include"asset/object_asset.h"
#include"core/meta/reflection/reflection.h"

namespace Mage {
	REFLECTION_TYPE(SceneAsset)
	CLASS(SceneAsset, Fields) {
		REFLECTION_BODY(SceneAsset)
	public:
		std::string name;
		std::vector<ObjectAsset> objects;
	};
}