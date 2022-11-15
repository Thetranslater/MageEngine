#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//每一个description对应一个primitive
	struct VkRenderMeshDescription {
		//mesh
		std::array<std::tuple<int, int, int>, 7> m_mesh_data_offset_infos; //bufferstride, offset, count. 6和vertex属性顺序对应, 7是indices
		//material
		float m_metallic_factor{ 1.f };
		float m_roughness_factor{ 1.f };

		int m_base_color_index{ -1 };
		int m_metallicroughness_index{ -1 };
		int m_normal_index{ -1 };

		glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };

		//params
		glm::mat4x4 m_matrix{ 1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f };

		VkRenderMeshDescription() {
			m_mesh_data_offset_infos.fill(std::make_tuple(-1, -1, -1));
		}
		
	};

	//mesh info in process deque
	struct VkRenderMeshInfo {

		VkRenderMeshURI m_buffer_uri;//buffer的位置，假设只有一个buffer

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //一个model info理论上对应一个gltf scene，一个scene包含许多mesh，每个mesh通过accessor访问buffer数据和贴图数据
	};

	struct VkRenderTextureInfo {
		std::vector<VkRenderTextureURI> m_uris;
	};
	//渲染数据信息，从CPU端传进，包含模型数据路径和贴图路径
	struct VkRenderModelInfo {
		GUID32 m_go_id;//TODO
		VkRenderMeshInfo m_mesh_info{};
		VkRenderTextureInfo m_textures_info{};
	};

	//TODO:model 代表着一个渲染实体，用GUID索引其mesh和material数据
	struct VkRenderModel {
		GUID32 m_model_guid32;

		glm::mat4x4 m_model_matrix;//user控制的transform组件，传递进GPU中。
		//TODO：bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user更改的color参数，传递进GPU中。

		VkRenderMeshDescription m_mesh_description;

		GUID32 m_mesh_guid32;
		std::vector<GUID32> m_texture_guid32s;
	};
}