#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

#include<glm-master/glm/glm.hpp>

namespace Mage {
	struct VkRenderMeshDescription {
		int m_index_offset;
		int m_index_count;
		int m_vertex_offset{ 0 }; //��Ϊÿһ��primitive��indices���ݶ��Ǵ�0��ʼ������Ϊ��������buffer���ҵ���ȷ��vertex������Ҫ�ҵ���ȷ��vertexoffset�ӵ�indices�����ϡ�
		
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

		VkRenderMeshURI m_buffer_uri;//buffer��λ��

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //����ǰ�������Ϣ��move��rendermodel
	};

	struct VkRenderTextureInfo {
		std::vector<VkRenderTextureURI> m_uris;
	};

	struct VkRenderModelInfo {
		GUID32 m_go_id;
		VkRenderMeshInfo m_mesh_info;
		VkRenderTextureInfo m_textures_info;
	};

	//TODO:model ������һ����Ⱦʵ�壬��GUID������mesh��material����
	struct VkRenderModel {
		GUID32 m_model_guid32;

		glm::mat4x4 m_model_matrix;//user���Ƶ�transform��������ݽ�GPU�С�
		//TODO��bounding box

		//glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };//user���ĵ�color���������ݽ�GPU�С�

		std::vector<VkRenderMeshDescription> m_mesh_descriptions;

		GUID32 m_mesh_guid32;
		std::vector<GUID32> m_texture_guid32s;
	};
}