#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

#include<glm-master/glm/glm.hpp>

namespace Mage {
	struct VkRenderMeshDescription {
		enum {
			MESH_USE_BASECOLOR_TEXUTRE = 1,
			MESH_USE_NORMAL_TEXTURE = 2,
			MESH_USE_METALLICROUGHNESS_TEXTURE = 4
		};

		int m_index_offset;
		int m_index_count;
		float m_submesh_metallic_factor{ 1.f };
		float m_submesh_roughness_factor{ 1.f };

		glm::vec4 m_submesh_base_color_factor{ 1.f,1.f,1.f,1.f };
		glm::mat4x4 m_submesh_matrix{ 1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f };
		unsigned char m_textures_use_info{ 0 }; //no use
	};

	//mesh info in process deque
	struct VkRenderMeshInfo {

		VkRenderMeshURI m_mesh_uri;//buffer的位置

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //可提前计算的信息，move给rendermodel
	};

	struct VkRenderMaterialInfo {
		VkRenderMaterialURI m_material_uri;
	};

	struct VkRenderModelInfo {
		GUID32 m_go_id;
		VkRenderMeshInfo m_mesh_info;
		VkRenderMaterialInfo m_material_info;
	};

	//TODO:model 代表着一个渲染实体，用GUID索引其mesh和material数据
	struct VkRenderModel {
		GUID32 m_model_guid32;

		glm::mat4x4 m_model_matrix;//user控制的transform组件，传递进GPU中。
		//TODO：bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user更改的color参数，传递进GPU中。

		std::vector<VkRenderMeshDescription> m_mesh_descriptions;

		GUID32 m_mesh_guid32;
		GUID32 m_material_guid32;
	};
}