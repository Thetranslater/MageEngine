#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//TODO:model 代表着一个渲染实体，用GUID索引其mesh和material数据
	struct VkRenderModel {
		GUID32 m_model_guid32;

		glm::mat4x4 m_model_matrix;
		//TODO：bounding box

		bool m_double_sided{ false };
		glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };

		GUID32 m_mesh_guid32;
		GUID32 m_material_guid32;
	};
}