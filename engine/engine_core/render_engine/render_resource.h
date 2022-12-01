#pragma once
#include<vector>
#include<array>
#include<variant>
#include<memory>
#include<map>

#include<glm-master/glm/glm.hpp>
#include<vulkan/vulkan.h>

#include<core/hash.h>

namespace Mage {

	struct VkRenderTexture {
		VkImage m_texture{VK_NULL_HANDLE};
		VkDeviceMemory m_texture_memory{VK_NULL_HANDLE};
		VkImageView m_texture_view{VK_NULL_HANDLE};
		VkSampler m_sampler{ VK_NULL_HANDLE };

		unsigned int ref_count{ 0 };
	};

	struct VkRenderMeshURI {
		std::string m_uri;
		bool operator==(const VkRenderMeshURI & rh) const {
			return m_uri == rh.m_uri;
		}
	};

	//TODO: submesh
	struct VkRenderMesh {
		VkBuffer m_index_buffer{VK_NULL_HANDLE};
		VkDeviceMemory m_index_memory{VK_NULL_HANDLE};

		VkBuffer m_vertex_buffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_vertex_memory{ VK_NULL_HANDLE };
	};

	//for gltf
	struct VkRenderBuffer {
		VkBuffer m_bi_data{ VK_NULL_HANDLE };
		VkDeviceMemory m_bi_data_memory{ VK_NULL_HANDLE };

		unsigned int ref_count{ 0 };
	};

	struct VkRenderTextureURI {
		std::string m_uri;
		bool is_srgb{ false };

		bool operator==(const VkRenderTextureURI& rh) const{
			return rh.m_uri == m_uri;
		}
	};

	struct GlobalUpdatedBuffer {
		VkBuffer m_buffer{VK_NULL_HANDLE};
		VkDeviceMemory m_buffer_memory{VK_NULL_HANDLE};
		void* m_followed_camera_updated_data_pointer{ nullptr };
	};

	class VulkanRHI;
	class Buffer;
	class Texture;
	class RenderResource {
	public:
		using GUID32 = uint32_t;
		using IO_Buffer = std::variant<VkRenderBuffer, Buffer>;
		using IO_Texture = std::variant<VkRenderTexture, Texture>;
	public:
		void initialize(VulkanRHI*);

		bool hasMesh(const GUID32& guid);
		bool hasTexture(const GUID32& guid);

		bool getOrCreateRenderResource(VulkanRHI*, GUID32& guid, IO_Buffer& param);
		bool getOrCreateRenderResource(VulkanRHI*, GUID32& guid, IO_Texture& param);
	public:
		std::map<GUID32, VkRenderBuffer>	m_guid_buffer_map;
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
}
