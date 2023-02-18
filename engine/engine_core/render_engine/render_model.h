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
		ID m_model_guid32;

		glm::mat4x4 m_model_matrix;//user控制的transform组件，传递进GPU中。
		//TODO：bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user更改的color参数，传递进GPU中。

		VkRenderMeshDescription m_mesh_description;

		ID m_mesh_combined_guid64;
		ID m_material_guid64;
	};
}