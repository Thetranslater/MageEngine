#pragma once
#include<vector>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include<core/macro.h>
#include<tinygITF/tiny_gltf.h>

#include<vulkan/vulkan.h>

#include<engine_core/render_engine/render_resource.h>
#include<engine_core/render_engine/render_model.h>

/*
* Ŀǰ�Ѿ�������tinygltfͷ�ļ�������ݣ�ʵ����buffer���ݺ�image�����ӳټ��أ�Ŀǰֻ���ش洢�����ļ������·�������ع����ӳٵ�����������£�tick()��������
* ��Ҫ���������Դ���ڵ����ݽṹ������uri���ݡ�
*/

namespace Mage {

	class VulkanRHI;

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

		bool loadTextureData(const std::string& base_dir);

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
		int m_material;
		int m_indices;
		int m_mode;
	};

	class Mesh {
	public:
		void loadFromgLTF_Mesh(tinygltf::Mesh& mesh);
	public:
		std::vector<Primitive> m_primitives;
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
	private:
		std::vector<unsigned char> m_data;
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

		std::vector<float> m_matrix;

		int m_parent{ -1 };
	};

	//TODO:PBR,����Ϊ��render resource��Խ�
	class Model {
	public:
		void loadFromgLTF_Model(tinygltf::Model& gltf_model);

		void loadFromgLTF_Model(tinygltf::Model&& gltf_model);

		//TODO:render system interact the resources by this.And NONE FINISH!!!
		VkRenderModel asVulkanRenderModel(VulkanRHI* rhi);

		std::vector<Mesh> m_meshes;
		std::vector<Accessor> m_accessors;
		//std::vector<Buffer> m_buffers;
		std::vector<BufferView> m_buffer_views;
		std::vector<Texture> m_textures;
		std::vector<Node> m_nodes;
		//std::vector<Sampler> m_samplers;
		std::vector<std::string> m_buffer_relative_uris;
		std::vector<std::string> m_texture_relative_uris;
	};

	//Animation asset types
}