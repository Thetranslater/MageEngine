#pragma once

#include"engine_core/render_engine/resource_swap_header.h"
#include"engine_core/render_engine/render_resource.h"

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//this is a bad name, a gltf model create a VkRenderModelInfo which including primitives.
	struct VkRenderModelInfo {
		ID m_go_id;//TODO
		VkRenderMeshInfo m_mesh_info{};
		VkRenderImageInfo m_images_info{};
		VkRenderMaterialInfo m_materials_info{};
	};

	//A model represent a render primitive, a gameobject always including multiple primitive. 
	struct VkRenderModel {
		VkRenderModel() = default;
		VkRenderModel(int size) :m_mesh_descriptions(size) {}

		glm::mat4x4 m_model_matrix;//user控制的transform组件，传递进GPU中。
		//TODO：bounding box

		std::vector<VkRenderMeshDescription> m_mesh_descriptions;
	};
}