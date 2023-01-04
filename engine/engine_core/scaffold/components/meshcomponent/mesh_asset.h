#pragma once
#include"core/meta/reflection/reflection.h"

namespace Mage {
	REFLECTION_TYPE(MeshAsset)
	CLASS(MeshAsset, Fields) {
		REFLECTION_BODY(MeshAsset)
	public:
		std::string gltf_model_url; //gltfģ�͵��ļ�����ʽΪgltf.ע�⣺ÿ�θ��Ķ���Ҫ�ı��Ӧmesh component��isloadedΪfalsed.

		std::string albedo;
		std::string normal_map;
		std::string metallic_roughness;//material
	};
}

