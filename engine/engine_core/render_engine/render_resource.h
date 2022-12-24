#pragma once
#include<vector>
#include<array>
#include<variant>
#include<memory>
#include<map>

#include<glm-master/glm/glm.hpp>
#include<vulkan/vulkan.h>

#include<core/hash.h>

#include"engine_core/render_engine/render_guid.h"
#include"engine_core/render_engine/render_macro.h"
#include"engine_core/render_engine/resource_swap_header.h"

namespace Mage {

	struct VkRenderTexture {
		VkImage m_texture{VK_NULL_HANDLE};
		VkDeviceMemory m_texture_memory{VK_NULL_HANDLE};
		VkImageView m_texture_view{VK_NULL_HANDLE};
		VkSampler m_sampler{ VK_NULL_HANDLE };
	};

	//for gltf
	struct VkRenderMesh {
		VkBuffer m_bi_data{ VK_NULL_HANDLE };
		VkDeviceMemory m_bi_data_memory{ VK_NULL_HANDLE };
	};

	struct VkRenderMaterial {
		bool m_double_side{ false };
		VkDescriptorSet m_descriptor_set;
	};

	struct GlobalUpdatedBuffer {
		std::vector<VkBuffer> m_buffers;
		std::vector<VkDeviceMemory> m_buffer_memories;
		std::vector<void*> m_followed_camera_updated_data_pointers;
	};

	struct GlobalBufferPerFrameData {
		//camera data
		glm::mat4 m_camera_view_matrix{};
		glm::mat4 m_camera_perspective_matrix{};
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

	struct GlobalBufferPerDrawcallVertexShaderData {
		PerMeshVertexShaderData m_mesh_datas[MAGE_PERDRAWCALL_MAX_LIMIT];
	};

	struct GlobalBufferPerDrawcallFragmentShaderData {
		PerMeshFragmentShaderData m_frag_datas[MAGE_PERDRAWCALL_MAX_LIMIT];
	};

	class VulkanRHI;
	class Buffer;
	class Texture;
	struct VkRenderMaterialDescription;
	class RenderResource {
	public:
		using GUID32 = uint32_t;
		using IO_Buffer = std::variant<VkRenderMesh, Buffer>;
		using IO_Texture = std::variant<VkRenderTexture, Texture>;
		using IO_Material = std::variant<VkRenderMaterial, VkRenderMaterialDescription>;
	public:
		void initialize(VulkanRHI*);

		bool hasMesh(const GUID32& guid);
		bool hasTexture(const GUID32& guid);
		bool hasMaterial(const GUID64& guid);

		bool getOrCreateRenderResource(VulkanRHI*, GUID32& guid, IO_Buffer& param);
		bool getOrCreateRenderResource(VulkanRHI*, GUID32& guid, IO_Texture& param);
		bool getOrCreateRenderResource(VulkanRHI*, GUID64& guid, IO_Material& param);
	public:
		std::map<GUID32, VkRenderMesh>		m_guid_buffer_map;
		std::map<GUID32, VkRenderTexture>	m_guid_texture_map;
		std::map<GUID64, VkRenderMaterial>	m_guid_material_map;

		//global resources
		GlobalUpdatedBuffer m_global_updated_buffer;
	};
}
