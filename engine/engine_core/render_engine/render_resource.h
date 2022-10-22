#pragma once
#include<vector>
#include<array>
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

		std::array<VkBuffer, 6> m_vertex_attribute_buffers{VK_NULL_HANDLE};
		std::array<VkBuffer, 6> m_vertex_attribute_memories{VK_NULL_HANDLE};
	};

	struct VkRenderTextureURI {
		std::string m_uri;
		//TODO:emissive, occulusion

		bool operator==(const VkRenderTextureURI& rh) const{
			return rh.m_uri == m_uri;
		}
	};

	class RenderResource {
		using GUID32 = uint32_t;
	public:
		std::map<GUID32, VkRenderMesh>		m_guid_mesh_map;
		std::map<GUID32, VkRenderTexture>	m_guid_texture_map;
	};
}

namespace std {
	template<>
	struct hash<Mage::VkRenderMeshURI> {
		size_t operator()(const Mage::VkRenderMeshURI& uri) {
			return hash<std::string>{}(uri.m_uri);
		}
	};

	template<>
	struct hash<Mage::VkRenderTextureURI> {
		size_t operator()(const Mage::VkRenderTextureURI& uri) {
			return hash<std::string>{}(uri.m_uri);
		}
	};
}
