#pragma once

#include<array>
#include<vector>
#include<string>
#include<variant>
#include<glm-master/glm/glm.hpp>

#include"asset/resource_manager/asset_type.h"

#include"engine_core/render_engine/render_guid.h"

#include"core/hash.h"

namespace Mage {
	//ÿһ��description��Ӧһ��primitive
	struct VkRenderMeshDescription {
		//mesh
		std::array<std::tuple<uint32_t, uint32_t, uint32_t>, 7> m_mesh_data_offset_infos; //bufferstride, offset, count. 6��vertex����˳���Ӧ, 7��indices
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

	//mesh info in process deque
	struct VkRenderMeshURI {
		VkRenderMeshURI() = default;
		explicit VkRenderMeshURI(const std::string& path) :m_uri(path) {};

		std::string m_uri;
		bool operator==(const VkRenderMeshURI& rh) const {
			return m_uri == rh.m_uri;
		}
	};

	struct RawMeshData {
		//Ϊ�˸�raw data����buffer guid����Ҫ��������������
		std::string m_accessory;//��Ϊ��������Ƕ�ģ�����gltf�ļ�·��������buffer��index���㹻����Ψһ��GUID ��ʽΪ��path:{index}

		Buffer m_raw;

		bool operator==(const RawMeshData& rh) const {
			return m_accessory == rh.m_accessory;;
		}
	};

	struct VkRenderMeshInfo {

		std::variant<VkRenderMeshURI, RawMeshData> m_info;//�ٶ�ֻ��һ��buffer,��buffer�漰̫��Ŀ��������Ŀǰ�ݲ�����

		VkRenderMeshURI m_buffer_uri;//buffer��λ�ã�����ֻ��һ��buffer

		std::vector<unsigned char> m_buffer_data;//uri �� dataֻ�ܴ���һ��

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //һ��model info�����϶�Ӧһ��gltf scene��һ��scene�������mesh��ÿ��meshͨ��accessor����buffer���ݺ���ͼ����
	};


	//texture info in process queue
	struct VkRenderTextureURI {
		VkRenderTextureURI() = default;
		explicit VkRenderTextureURI(const std::string& path) :m_uri{ path } {}

		std::string m_uri;

		bool operator==(const VkRenderTextureURI& rh) const {
			return rh.m_uri == m_uri;
		}
	};
	//gltf��image������bufferview����ʽ����ͼ������
	struct RawTextureData {
		std::string m_accessory; //ͬraw buffer dataͬ�����������п��ܶ��accessory��Ӧͬһ��texture data�������ڴ��д�����ͬʵ����

		Texture m_raw;

		bool operator==(const RawTextureData& rh) const {
			return m_accessory == rh.m_accessory;;
		}
	};

	struct PieceInfo {
		std::variant<VkRenderTextureURI, RawTextureData> m_detail;

		bool is_srgb{ false };
	};

	struct VkRenderTextureInfo {
		std::vector<PieceInfo> m_infos;

		std::vector<VkRenderTextureURI> m_uris;
		std::vector<std::vector<unsigned char>> m_datas;
	};

	//material info
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

	template<>
	struct hash<Mage::RawMeshData> {
		size_t operator()(const Mage::RawMeshData& raw) const {
			return hash<std::string>{}(raw.m_accessory);
		}
	};

	template<>
	struct hash<Mage::RawTextureData> {
		size_t operator()(const Mage::RawTextureData& raw) const {
			return hash<std::string>{}(raw.m_accessory);
		}
	};
}