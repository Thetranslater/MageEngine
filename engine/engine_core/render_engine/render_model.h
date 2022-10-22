#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

#include<glm-master/glm/glm.hpp>

namespace Mage {
	struct VkRenderMeshDescription {
		int m_index_offset;
		int m_index_count;
		int m_vertex_offset{ 0 }; //因为每一个primitive的indices数据都是从0开始，所以为了在整个buffer中找到正确的vertex数据需要找到正确的vertexoffset加到indices数据上。
		
		float m_metallic_factor{ 1.f };
		float m_roughness_factor{ 1.f };

		int m_base_color_index{ -1 };
		int m_metallicroughness_index{ -1 };
		int m_normal_index{ -1 };

		glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };
		glm::mat4x4 m_matrix{ 1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f };
		
	};

	//mesh info in process deque
	struct VkRenderMeshInfo {

		VkRenderMeshURI m_buffer_uri;//buffer的位置

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //可提前计算的信息，move给rendermodel
	};

	struct VkRenderTextureInfo {
		std::vector<VkRenderTextureURI> m_uris;
	};

	struct VkRenderModelInfo {
		GUID32 m_go_id;
		VkRenderMeshInfo m_mesh_info;
		VkRenderTextureInfo m_textures_info;
	};

	//TODO:model 代表着一个渲染实体，用GUID索引其mesh和material数据
	struct VkRenderModel {
		GUID32 m_model_guid32;

		glm::mat4x4 m_model_matrix;//user控制的transform组件，传递进GPU中。
		//TODO：bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user更改的color参数，传递进GPU中。

		std::vector<VkRenderMeshDescription> m_mesh_descriptions;

		GUID32 m_mesh_guid32;
		std::vector<GUID32> m_texture_guid32s;
	};
}