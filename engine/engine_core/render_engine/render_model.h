#pragma once

#include"engine_core/render_engine/resource_swap_header.h"
#include"engine_core/render_engine/render_resource.h"

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//��Ⱦ������Ϣ����CPU�˴���������ģ������·������ͼ·��
	struct VkRenderModelInfo {
		ID m_go_id;//TODO
		VkRenderMeshInfo m_mesh_info{};
		VkRenderImageInfo m_images_info{};
		VkRenderMaterialInfo m_materials_info{};
	};

	//TODO:model ������һ����Ⱦʵ�壬��GUID������mesh��material����
	struct VkRenderModel {
		ID m_model_guid32;

		glm::mat4x4 m_model_matrix;//user���Ƶ�transform��������ݽ�GPU�С�
		//TODO��bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user���ĵ�color���������ݽ�GPU�С�

		VkRenderMeshDescription m_mesh_description;

		ID m_mesh_combined_guid64;
		ID m_material_guid64;
	};
}