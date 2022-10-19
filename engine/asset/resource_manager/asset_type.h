#pragma once
#include<vector>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include<core/macro.h>
#include<tinygITF/tiny_gltf.h>

#include<vulkan/vulkan.h>

#include<core/math/matrix4x4.h>
#include<core/math/vector3.h>
#include<core/math/vector4.h>

#include<engine_core/render_engine/render_resource.h>
#include<engine_core/render_engine/render_model.h>

/*
* 目前已经更改完tinygltf头文件相关内容，实现了buffer数据和image数据延迟加载，目前只加载存储数据文件的相对路径，加载功能延迟到后面组件更新（tick()）功能中
* 需要更改相关资源类内的数据结构，保存uri内容。
*/

namespace Mage {

	class VulkanRHI;
	struct VkRenderModelInfo;

	enum MageFormat {
		MAGE_FORMAT_UNDEFINED = 0,
		MAGE_FORMAT_R8G8B8_UNORM = 23, //align to vulkan format
		MAGE_FORMAT_R8G8B8_SNORM,
		MAGE_FORMAT_R8G8B8_SRGB = 29,
		MAGE_FORMAT_R8G8B8A8_UNORM = 37,
		MAGE_FORMAT_R8G8B8A8_SNORM, 
		MAGE_FORMAT_R8G8B8A8_SRGB = 43,
		MAGE_FORMAT_R32G32_UINT = 101,
		MAGE_FORMAT_R32G32_SINT,
		MAGE_FORMAT_R32G32_SFLOAT,
		MAGE_FORMAT_R32G32B32_UINT,
		MAGE_FORMAT_R32G32B32_SINT,
		MAGE_FORMAT_R32G32B32_SFLOAT,
		MAGE_FORMAT_R32G32B32A32_UINT,
		MAGE_FORMAT_R32G32B32A32_SINT,
		MAGE_FORMAT_R32G32B32A32_SFLOAT,
	};

	enum MageFilter {
		MAGE_FILTER_NEAREST = 0,
		MAGE_FILTER_LINEAR
	};

	enum MageSamplerAddressMode {
		MAGE_SAMPLER_ADDRESS_MODE_REPEAT = 0,
		MAGE_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
		MAGE_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		MAGE_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
		MAGE_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
	};

	//TODO:sampler
	class Sampler{
	public:
		VkSampler asVulkanSampler(VulkanRHI* rhi, VkSamplerCreateInfo* optional_info = nullptr);

		void loadFromgLTF_Sampler(tinygltf::Sampler& gltf_sampler);
	private:
		MageFilter m_magfilter{MageFilter::MAGE_FILTER_NEAREST};
		MageFilter m_minfilter{ MageFilter::MAGE_FILTER_NEAREST };
		MageFilter m_mipmapfilter{ MageFilter::MAGE_FILTER_NEAREST };

		MageSamplerAddressMode m_address_mode_u{ MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT };
		MageSamplerAddressMode m_address_mode_v{ MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT };
		MageSamplerAddressMode m_address_mode_w{ MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT };
	};

	class Texture {
	public:
		uint32_t getHeight() { return m_height; }
		uint32_t getWidth() { return m_width; }

		VkRenderTexture asVulkanRenderTexture(VulkanRHI* rhi);

		void loadFromgLTF_Image(tinygltf::Image& gltf_image, tinygltf::Sampler& gltf_sampler);

		bool load(const std::string& base_dir, std::string& err, std::string& warn);

	public:
		uint32_t m_height{0};
		uint32_t m_width{0};
		uint32_t m_layer_counts{ 0 };
		uint32_t m_mipmap_levels{ 0 };
		MageFormat m_format{ MageFormat::MAGE_FORMAT_UNDEFINED };
		Sampler m_combined_sampler;

		std::string m_uri;

		//move from gltf_image
		std::vector<unsigned char> m_data;
	};

	class Image;

	class Primitive {
	public:
		void loadFromgLTF_Primitive(tinygltf::Primitive& primitive);
	public:
		std::map<std::string, int> m_attributes;
		int m_material{ -1 };
		int m_indices{ -1 };
		int m_mode{ -1 };
	};

	class Mesh {
	public:
		void loadFromgLTF_Mesh(tinygltf::Mesh& mesh);
	public:
		std::vector<Primitive> m_primitives;
	};

	struct TextureInfo {
		int m_index{ -1 };
		int m_texCoord{ 0 };
	};
	struct NormalTextureInfo {
		int		m_index{ -1 };
		int		m_texCoord{ 0 };
		float	m_scale{ 1.f };
	};
	struct PbrMetallicRoughness {
		Vector4		m_base_color_factor{ 1.f,1.f,1.f,1.f };
		float		m_metallic_factor{ 1.f };
		float		m_roughness_factor{ 1.f };
		TextureInfo m_base_color_texture;
		TextureInfo m_metallic_roughness_texture;
	};
	struct OcclusionTextureInfo {
		int		m_index{ -1 };
		int		m_texCoord{ 0 };
		float	m_strength{ 1.f };
	};

	class Material {
	public:
		void loadFromgLTF_Material(tinygltf::Material& material);
	public:
		Vector3					m_emissive_factor{ 0.f,0.f,0.f };
		PbrMetallicRoughness	m_pbr_metallic_roughness;
		NormalTextureInfo		m_normal_texture;
		OcclusionTextureInfo	m_occlusion_texture;
		TextureInfo				m_emissive_texture;
	};

	class Accessor {
	public:
		enum ComponentType {
			COMPONENT_NONE = 0,
			SBYTE = 5120, //match for gltf
			UBYTE,
			SSHORT,
			USHORT,
			UINT = 5125,
			FLOAT
		};

		enum Type{
			NONE = 0,
			VEC2 = 2, //match for tinygltf
			VEC3,
			VEC4,
			MAT2 = 32 + 2,
			MAT3,
			MAT4,
			SCALAR= 64 + 1
		};

		void loadFromgLTF_Accessor(tinygltf::Accessor& accessor);

	public:
		int m_buffer_view{ -1 };
		size_t m_byte_offset{ 0 };
		ComponentType m_component_type{ ComponentType::COMPONENT_NONE };
		Type m_type{ Type::NONE };
		size_t m_count{ 0 };

		std::vector<double> m_max;
		std::vector<double> m_min;

	};

	class Buffer {
	public:
		VkBuffer asVulkanBuffer(VulkanRHI* rhi);
		void loadFromgLTF_Buffer(tinygltf::Buffer& buffer);
		unsigned char* data() { return m_data.data(); }
		uint32_t size() { return static_cast<uint32_t>(m_data.size()); }
		bool load(const std::string& base_dir, std::string& err, std::string& warn);
	public:
		std::vector<unsigned char> m_data;
		std::string m_uri;
	};

	class BufferView {
	public:
		enum Target{
			TARGET_NONE = 0,
			TARGET_VERTEX = 34962,
			TARGET_INDEX
		};

		void loadFromgLTF_BufferView(tinygltf::BufferView& buffer_view);
	public:
		int m_buffer{ -1 }; //index to buffer
		size_t m_byte_offset{ 0 }; //offset in buffer
		size_t m_byte_length{ 0 }; //length of buffer view
		size_t m_byte_stride{ 0 }; //stride of vertex attribute

		Target m_target{ Target::TARGET_NONE }; //"ELEMENT_ARRAY_BUFFER - 34963 - indices buffer, ARRAY_BUFFER - 34962 - attributes buffer"
	};

	class Node {
	public:
		void loadFromgLTF_Node(tinygltf::Node& node);
	public:
		int m_camera{-1};
		int m_mesh{-1};
		std::vector<int> m_children;

		std::vector<float> m_rotation;
		std::vector<float> m_scale;
		std::vector<float> m_translation;

		std::vector<float> m_matrix; //column-major

		int m_parent{ -1 };
	};

	//TODO:PBR,更改为于render resource层对接。未来需要全部抽象为gameobject，mesh作为component实现。
	class Model {
	public:
		void loadFromgLTF_Model(tinygltf::Model& gltf_model);

		void loadFromgLTF_Model(tinygltf::Model&& gltf_model);

		//从model中获得rendermodel的信息，主要处理node节点和primitive
		VkRenderModelInfo getVkRenderModelInfo();

		std::string m_model_filepath;

		std::vector<Mesh> m_meshes;
		std::vector<Accessor> m_accessors;
		std::vector<Buffer> m_buffers;
		std::vector<BufferView> m_buffer_views;
		std::vector<Texture> m_textures;
		std::vector<Node> m_nodes;
		std::vector<Material> m_materials;
		//std::vector<Sampler> m_samplers;

		//TODO:等待删除
		GUID32 m_go_id;
	private:
		//for mesh
		void processNode(const std::vector<Node>& nodes, int curr_index, const Matrix4x4& parent_matrix, const std::function<void(const int&, const Matrix4x4&)>& process_func);
	};

	//Animation asset types
}