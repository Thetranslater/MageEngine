#pragma once
#include"core/meta/reflection/reflection.h"

namespace Mage {
	REFLECTION_TYPE(MeshAsset)
	CLASS(MeshAsset, Fields) {
		REFLECTION_BODY(MeshAsset)
	public:
		std::string gltf_model_url; //gltf模型的文件，格式为gltf.注意：每次更改都需要改变对应mesh component的isloaded为falsed.

		std::string albedo;
		std::string normal_map;
		std::string metallic_roughness;//material
	};
}

