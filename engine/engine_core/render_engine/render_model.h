#pragma once

#include"engine_core/render_engine/resource_swap_header.h"
#include"engine_core/render_engine/render_resource.h"

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//渲染数据信息，从CPU端传进，包含模型数据路径和贴图路径
	struct VkRenderModelInfo {
		ID m_go_id;//TODO
		VkRenderMeshInfo m_mesh_info{};
		VkRenderImageInfo m_images_info{};
		VkRenderMaterialInfo m_materials_info{};
	};

	//TODO:model 代表着一个渲染实体，用GUID索引其mesh和material数据
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