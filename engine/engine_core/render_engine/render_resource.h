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
	};

	struct VkRenderTextureURI {
		std::string m_uri;

		bool operator==(const VkRenderTextureURI& rh) const{
			return rh.m_uri == m_uri;
		}
	};

	class VulkanRHI;
	class RenderResource {
		using GUID32 = uint32_t;
	public:
		bool hasMesh(const GUID32& guid);
		bool hasTexture(const GUID32& guid);

		bool getOrCreateRenderResource(VulkanRHI* rhi, GUID32& guid, std::variant<VkRenderBuffer,std::vector<unsigned char>>& param);
		bool getOrCreateRenderResource(VulkanRHI* rhi, GUID32& guid, std::variant<VkRenderTexture,std::vector<unsigned char>>& param);
	public:
		std::map<GUID32, VkRenderBuffer>	m_guid_buffer_map;
		std::map<GUID32, VkRenderTexture>	m_guid_texture_map;
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
