#pragma once
#include<vector>
#include<array>
#include<variant>
#include<memory>
#include<map>

#include<glm-master/glm/glm.hpp>
#include<vulkan/vulkan.h>

#include<core/hash.h>

#include<engine_core/render_engine/render_guid.h>

namespace Mage {

	constexpr static int per_drawcall_max_instances = 60;

	struct VkRenderTexture {
		VkImage m_texture{VK_NULL_HANDLE};
		VkDeviceMemory m_texture_memory{VK_NULL_HANDLE};
		VkImageView m_texture_view{VK_NULL_HANDLE};
		VkSampler m_sampler{ VK_NULL_HANDLE };
	};

	struct VkRenderMeshURI {
		std::string m_uri;
		bool operator==(const VkRenderMeshURI & rh) const {
			return m_uri == rh.m_uri;
		}
	};

	//for gltf
	struct VkRenderMesh {
		VkBuffer m_bi_data{ VK_NULL_HANDLE };
		VkDeviceMemory m_bi_data_memory{ VK_NULL_HANDLE };
	};

	struct VkRenderTextureURI {
		std::string m_uri;
		bool is_srgb{ false };

		bool operator==(const VkRenderTextureURI& rh) const{
			return rh.m_uri == m_uri;
		}
	};

	struct VkRenderPartMesh {
		GUID32 m_mesh_guid;
		GUID32 m_part_index;

		bool operator==(const VkRenderPartMesh& rh) const {
			return m_mesh_guid == rh.m_mesh_guid && m_part_index == rh.m_part_index;
		}
	};

	struct GlobalUpdatedBuffer {
		VkBuffer m_buffer{VK_NULL_HANDLE};
		VkDeviceMemory m_buffer_memory{VK_NULL_HANDLE};
		void* m_followed_camera_updated_data_pointer{ nullptr };
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
		PerMeshVertexShaderData m_mesh_datas[per_drawcall_max_instances];
	};

	struct GlobalBufferPerDrawcallFragmentShaderData {
		PerMeshFragmentShaderData m_frag_datas[per_drawcall_max_instances];
	};

	class VulkanRHI;
	class Buffer;
	class Texture;
	class RenderResource {
	public:
		using GUID32 = uint32_t;
		using IO_Buffer = std::variant<VkRenderMesh, Buffer>;
		using IO_Texture = std::variant<VkRenderTexture, Texture>;
	public:
		void initialize(VulkanRHI*);

		bool hasMesh(const GUID32& guid);
		bool hasTexture(const GUID32& guid);

		bool getOrCreateRenderResource(VulkanRHI*, GUID32& guid, IO_Buffer& param);
		bool getOrCreateRenderResource(VulkanRHI*, GUID32& guid, IO_Texture& param);
	public:
		std::map<GUID32, VkRenderMesh>	m_guid_buffer_map;
		std::map<GUID32, VkRenderTexture>	m_guid_texture_map;

		//global resources
		GlobalUpdatedBuffer m_global_updated_buffer;
	};
}

namespace std {
	template<>
	struct hash<Mage::VkRenderMeshURI> {
		size_t operator()(const Mage::VkRenderMeshURI& uri) const {
			return hash<std::string>{}(uri.m_uri);
		}
	};

	template<>
	struct hash<Mage::VkRenderTextureURI> {
		size_t operator()(const Mage::VkRenderTextureURI& uri) const {
			return hash<std::string>{}(uri.m_uri);
		}
	};

	template<>
	struct hash<Mage::VkRenderPartMesh> {
		size_t operator()(const Mage::VkRenderPartMesh& part) const {
			size_t hash = part.m_mesh_guid;
			Mage::hash_combine(hash, part.m_part_index);
			return hash;
		}
	};
}
