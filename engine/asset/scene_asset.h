#pragma once
#include<string>

#include"asset/object_asset.h"
#include"core/meta/reflection/reflection.h"

namespace Mage {
	REFLECTION_TYPE(SceneNode)
	CLASS(SceneNode, Fields) {
		REFLECTION_BODY(SceneNode)
	public:
		int parent{ -1 };
		std::vector<int> children;
	};
	
	//requirements: the size of nodes must match the size of objects, they are in one-to-one relationship;
	REFLECTION_TYPE(SceneAsset)
	CLASS(SceneAsset, Fields) {
		REFLECTION_BODY(SceneAsset)
	public:
		std::string name;
		std::vector<int> roots;
		std::vector<SceneNode> nodes;
		std::vector<ObjectAsset> objects;
	};
}