#pragma once
#include<vector>
#include<array>
#include<variant>
#include<memory>
#include<map>

#include<glm-master/glm/glm.hpp>
#include<vulkan/vulkan.h>

#include<core/hash.h>

#include"engine_core/function/id_allocator/id_allocator.h"
#include"engine_core/render_engine/render_macro.h"

namespace Mage {
	//push constant
	struct PerMaterialPushConstant {
		int m_alphaMode{0};
		float m_alphaCutOff{ 0.5f };
	};

	struct VkRenderTexture {
		VkImage m_texture{VK_NULL_HANDLE};
		VkDeviceMemory m_texture_memory{VK_NULL_HANDLE};
		VkImageView m_texture_view{VK_NULL_HANDLE};
		VkSampler m_sampler{ VK_NULL_HANDLE };//remove?
	};

	struct VkRenderMesh {
		VkBuffer m_bi_data{ VK_NULL_HANDLE };
		VkDeviceMemory m_bi_data_memory{ VK_NULL_HANDLE };
	};

	struct VkRenderMaterial {
		bool m_double_side{ false };
		VkDescriptorSet m_descriptor_set;
		PerMaterialPushConstant m_push_constant;
	};

	struct GlobalUpdatedBuffer {
		std::vector<VkBuffer> m_buffers;
		std::vector<VkDeviceMemory> m_buffer_memories;
		std::vector<void*> m_followed_camera_updated_data_pointers;
	};
	
	struct PerDirectionalLightData {
		glm::vec3 m_direction;
		float m_intensity;
		glm::vec3 m_color;
		float _unused_blank_1;
	};

	struct PerPointLightData {
		glm::vec3 m_position;
		float m_intensity;
		glm::vec3 m_color;
		float _unused_blank_1; //radius
	};

	//per frame
	struct GlobalBufferPerFrameData {
		//camera data
		glm::mat4 m_camera_view_matrix{};
		glm::mat4 m_camera_perspective_matrix{};
		//lights data
		PerDirectionalLightData m_directional_lights[8];
		PerPointLightData m_point_lights[8];
	};

	struct PerMeshVertexShaderData {
		//TODO:vectex blending
		glm::mat4 m_matrix;
	};

	struct PerMeshFragmentShaderData {
		float m_metallic_factor{ 1.f };
		float m_roughness_factor{ 1.f };
		float _unused_blank_1;
		float _unused_blank_2;
		glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };
	};

	struct PerMeshShaderData {
		PerMeshVertexShaderData m_vertex_data;
		PerMeshFragmentShaderData m_fragment_data;
	};

	//per drawcall
	struct GlobalBufferPerDrawcallData {
		PerMeshShaderData m_data[MAGE_PERDRAWCALL_MAX_LIMIT] ;
	};

	class VulkanRHI;
	class Buffer;
	class Image;
	struct VkRenderMaterialDescription;
	class RenderResource {
	public:
		//交互数据，可以同时作为输入和输出进行数据的存储和读取
		using IO_Buffer = std::variant<VkRenderMesh, Buffer>;
		using IO_Texture = std::variant<VkRenderTexture, Image>;
		using IO_Material = std::variant<VkRenderMaterial, VkRenderMaterialDescription>;
	public:
		void initialize(VulkanRHI*);

		bool hasMesh(const ID& guid);
		bool hasTexture(const ID& guid);
		bool hasMaterial(const ID& guid);

		bool getOrCreateRenderResource(VulkanRHI*, ID& guid, IO_Buffer& param);
		bool getOrCreateRenderResource(VulkanRHI*, ID& guid, IO_Texture& param);
		bool getOrCreateRenderResource(VulkanRHI*, ID& guid, IO_Material& param);
	public:
		std::map<ID, VkRenderMesh>		m_guid_buffer_map;
		std::map<ID, VkRenderTexture>	m_guid_texture_map;
		std::map<ID, VkRenderMaterial>	m_guid_material_map;

		//global resources
		GlobalUpdatedBuffer m_global_updated_buffer;
	};
}
