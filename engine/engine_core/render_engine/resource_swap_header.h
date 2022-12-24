#pragma once

#include<array>
#include<vector>
#include<string>

#include"engine_core/render_engine/render_guid.h"

#include"core/hash.h"

#include<glm-master/glm/glm.hpp>

namespace Mage {
	//每一个description对应一个primitive
	struct VkRenderMeshDescription {
		//mesh
		std::array<std::tuple<uint32_t, uint32_t, uint32_t>, 7> m_mesh_data_offset_infos; //bufferstride, offset, count. 6和vertex属性顺序对应, 7是indices
		//material
		float m_metallic_factor{ 1.f };
		float m_roughness_factor{ 1.f };
		int m_material_index{ -1 };

		glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };

		//params
		glm::mat4x4 m_matrix{ 1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f };

		VkRenderMeshDescription() {
			m_mesh_data_offset_infos.fill(std::make_tuple(-1, -1, -1));
		}

	};

	struct VkRenderMaterialDescription {
		bool m_double_side{ false };
		uint32_t m_base_color_texture_index{ 0xffffffff };
		uint32_t m_metallic_roughness_texture_index{ 0xffffffff };
		uint32_t m_normal_texture_index{ 0xffffffff };
		uint32_t m_occlusion_texture_index{ 0xffffffff };

		bool operator==(const VkRenderMaterialDescription& r) const {
			return r.m_double_side == m_double_side &&
				r.m_base_color_texture_index == m_base_color_texture_index &&
				r.m_metallic_roughness_texture_index == m_metallic_roughness_texture_index &&
				r.m_normal_texture_index == m_normal_texture_index &&
				r.m_occlusion_texture_index == m_occlusion_texture_index;
		}
	};

	struct VkRenderTextureURI {
		std::string m_uri;
		bool is_srgb{ false };

		bool operator==(const VkRenderTextureURI& rh) const {
			return rh.m_uri == m_uri;
		}
	};

	struct VkRenderMeshURI {
		std::string m_uri;
		bool operator==(const VkRenderMeshURI& rh) const {
			return m_uri == rh.m_uri;
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

	struct VkRenderMaterialInfo {
		std::vector<VkRenderMaterialDescription> m_infos;
	};

	struct VkRenderPartMesh {
		GUID32 m_mesh_guid;
		GUID32 m_part_index;

		bool operator==(const VkRenderPartMesh& rh) const {
			return m_mesh_guid == rh.m_mesh_guid && m_part_index == rh.m_part_index;
		}
	};
}

namespace std {
	template <>
	struct hash<Mage::VkRenderMaterialDescription> {
		size_t operator()(const Mage::VkRenderMaterialDescription& material) const {
			size_t hash = material.m_base_color_texture_index;
			Mage::hash_combine(hash, material.m_metallic_roughness_texture_index, material.m_normal_texture_index, material.m_occlusion_texture_index);
			return hash;
		}
	};

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