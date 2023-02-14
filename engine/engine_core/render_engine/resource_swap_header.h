#pragma once

#include<array>
#include<vector>
#include<string>
#include<variant>
#include<glm-master/glm/glm.hpp>

#include"asset/resource_manager/asset_type.h"

#include"core/hash.h"
#include"core/macro.h"

namespace Mage {

	using ID = uint64_t;

	//ÿһ��description��Ӧһ��primitive
	struct VkRenderMeshAttributeDescription {
		STATIC_DATA int m_buffer_index{-1};

		STATIC_DATA int m_stride{-1};
		STATIC_DATA int	m_offset{-1};
		STATIC_DATA int m_count{-1};
	};

	struct VkRenderMeshDescription {
		//mesh
		STATIC_DATA std::array<VkRenderMeshAttributeDescription, 7> m_attribute_infos{}; //6��vertex����˳���Ӧ, 7��indices
		//material
		DYNAMIC_DATA float m_metallic_factor{ 1.f };
		DYNAMIC_DATA float m_roughness_factor{ 1.f };
		DYNAMIC_DATA glm::vec4 m_base_color_factor{ 1.f,1.f,1.f,1.f };

		DYNAMIC_DATA int m_material_index{ -1 };

		//params
		DYNAMIC_DATA glm::mat4x4 m_matrix{ 1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f };
	};

	struct VkRenderMaterialDescription {
		STATIC_DATA bool m_double_side{ false };
		STATIC_DATA uint32_t m_base_color_texture_index{ 0xffffffff };
		STATIC_DATA Sampler m_base_color_texture_sampler{};

		STATIC_DATA uint32_t m_metallic_roughness_texture_index{ 0xffffffff };
		STATIC_DATA Sampler m_metallic_roughness_texture_sampler{};

		STATIC_DATA uint32_t m_normal_texture_index{ 0xffffffff };
		STATIC_DATA Sampler m_normal_texture_sampler{};

		STATIC_DATA uint32_t m_occlusion_texture_index{ 0xffffffff };
		STATIC_DATA Sampler m_occlusion_texture_sampler{};

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

		STATIC_DATA std::string m_uri;
		bool operator==(const VkRenderMeshURI& rh) const {
			return m_uri == rh.m_uri;
		}
	};

	struct RawMeshData {
		//Ϊ�˸�raw data����buffer guid����Ҫ��������������
		STATIC_DATA std::string m_accessory;//��Ϊ��������Ƕ�ģ�����gltf�ļ�·��������buffer��index���㹻����Ψһ��GUID ��ʽΪ��path:{index}

		STATIC_DATA Buffer m_raw;

		bool operator==(const RawMeshData& rh) const {
			return m_accessory == rh.m_accessory;;
		}
	};

	struct VkRenderMeshInfo {

		std::vector<std::variant<VkRenderMeshURI, RawMeshData>> m_infos;//�ٶ�ֻ��һ��buffer,��buffer�漰̫��Ŀ��������Ŀǰ�ݲ�����

		std::vector<VkRenderMeshDescription> m_transfer_mesh_descriptions; //һ��model info�����϶�Ӧһ��gltf scene��һ��scene�������mesh��ÿ��meshͨ��accessor����buffer���ݺ���ͼ����
	};

	//texture info in process queue
	struct VkRenderImageURI {
		VkRenderImageURI() = default;
		explicit VkRenderImageURI(const std::string& path) :m_uri{ path } {}

		STATIC_DATA std::string m_uri;

		bool operator==(const VkRenderImageURI& rh) const {
			return rh.m_uri == m_uri;
		}
	};
	//gltf��image������bufferview����ʽ����ͼ������
	struct RawImageData {
		STATIC_DATA std::string m_accessory; //ͬraw buffer dataͬ�����������п��ܶ��accessory��Ӧͬһ��texture data�������ڴ��д�����ͬʵ����

		STATIC_DATA Image m_raw;

		bool operator==(const RawImageData& rh) const {
			return m_accessory == rh.m_accessory;;
		}
	};

	struct PieceInfo {
		STATIC_DATA std::variant<VkRenderImageURI, RawImageData> m_detail;

		STATIC_DATA bool is_srgb{ false };
	};

	struct VkRenderImageInfo {
		STATIC_DATA std::vector<PieceInfo> m_infos;
	};

	//material info
	struct VkRenderMaterialInfo {
		std::vector<VkRenderMaterialDescription> m_infos;
	};

	struct VkRenderPartMesh {
		ID m_mesh_guid;
		ID m_part_index;

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
	struct hash<Mage::VkRenderImageURI> {
		size_t operator()(const Mage::VkRenderImageURI& uri) const {
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
	struct hash<Mage::RawImageData> {
		size_t operator()(const Mage::RawImageData& raw) const {
			return hash<std::string>{}(raw.m_accessory);
		}
	};
}