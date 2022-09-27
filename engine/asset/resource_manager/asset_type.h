#pragma once
#include<vector>

#include<core/macro.h>

namespace Mage {

	class VulkanRHI;

	enum MageFormat {
		MAGE_FORMAT_UNDEFINED = 0,
		MAGE_FORMAT_R8G8B8_UNORM = 23, //align to vulkan format
		MAGE_FORMAT_R8G8B8_SRGB = 29,
		MAGE_FORMAT_R8G8B8A8_UNORM = 37,
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

	class Texture {
	public:
		uint32_t getHeight() { return m_height; }
		uint32_t getWidth() { return m_width; }

		//TODO:return a tuple includes image, image view, sampler
		VkImage asVulkanImage(VulkanRHI* rhi);

		void fromgITFTexture();

	private:
		uint32_t m_height{0};
		uint32_t m_width{0};
		uint32_t m_layer_counts{ 0 };
		uint32_t m_mipmap_levels{ 0 };
		MageFormat m_format{ MageFormat::MAGE_FORMAT_UNDEFINED };

		//data
		void* m_data{ nullptr };
		uint32_t m_size{ 0 }; //size in bytes
	};

	class Image;

	struct MeshData {
		//TODO:vertex buffer, index buffer
	};

	//vertex defination
	/*
	* struct Vertex;
	*/

	class Mesh;

	//TODO:PBR
	class Model {
	public:
		std::vector<Mesh> m_meshes;
		std::vector<Texture> m_textures;
	};

	//Animation asset types
}