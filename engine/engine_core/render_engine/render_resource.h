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

	struct VkRenderMaterialURI {
		std::string m_base_color_uri;
		std::string m_metallic_roughness_uri;
		std::string m_normal_uri;
		//TODO:emissive, occulusion

		bool operator==(const VkRenderMaterialURI& rh) const{
			return rh.m_base_color_uri == m_base_color_uri &&
				rh.m_metallic_roughness_uri == m_metallic_roughness_uri &&
				rh.m_normal_uri == m_normal_uri;
		}

		size_t getHash() const{
			size_t value;
			hash_combine(value, m_base_color_uri, m_metallic_roughness_uri, m_normal_uri);
			return value;
		}
	};

	//TODO:PBR material
	struct VkRenderMaterial {
		VkRenderTexture m_base_color_texture;
		VkRenderTexture m_metallic_roughness_texture;
		VkRenderTexture	m_normal_texture;
		//TODO:emissive, occlusion
		VkDescriptorSet m_material_descriptor_set;
	};

	class RenderResource {
		using GUID32 = uint32_t;
	public:
		std::map<GUID32, VkRenderMesh>		m_guid_mesh_map;
		std::map<GUID32, VkRenderMaterial>	m_guid_material_map;
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
	struct hash<Mage::VkRenderMaterialURI> {
		size_t operator()(const Mage::VkRenderMaterialURI& uri) {
			return uri.getHash();
		}
	};
}
