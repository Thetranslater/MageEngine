#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//ÿһ��description��Ӧһ��primitive
	struct VkRenderMeshDescription {
		//mesh
		std::array<std::tuple<int, int, int>, 7> m_mesh_data_offset_infos; //bufferstride, offset, count. 6��vertex����˳���Ӧ, 7��indices
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

		VkRenderMeshURI m_buffer_uri;//buffer��λ�ã�����ֻ��һ��buffer

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //һ��model info�����϶�Ӧһ��gltf scene��һ��scene�������mesh��ÿ��meshͨ��accessor����buffer���ݺ���ͼ����
	};

	struct VkRenderTextureInfo {
		std::vector<VkRenderTextureURI> m_uris;
	};
	//��Ⱦ������Ϣ����CPU�˴���������ģ������·������ͼ·��
	struct VkRenderModelInfo {
		GUID32 m_go_id;//TODO
		VkRenderMeshInfo m_mesh_info{};
		VkRenderTextureInfo m_textures_info{};
	};

	//TODO:model ������һ����Ⱦʵ�壬��GUID������mesh��material����
	struct VkRenderModel {
		GUID32 m_model_guid32;

		glm::mat4x4 m_model_matrix;//user���Ƶ�transform��������ݽ�GPU�С�
		//TODO��bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user���ĵ�color���������ݽ�GPU�С�

		VkRenderMeshDescription m_mesh_description;

		GUID32 m_mesh_guid32;
		std::vector<GUID32> m_texture_guid32s;
	};
}