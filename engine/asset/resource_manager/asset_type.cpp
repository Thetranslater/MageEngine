#include<assert.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include<asset/resource_manager/asset_type.h>

#include<engine_core/render_engine/renderer/vulkanInfo.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>
#include<engine_core/platform/file_system.h>


namespace Mage {
	//buffer
	VkBuffer Buffer::asVulkanBuffer(VulkanRHI* rhi) {
		assert(!m_data.empty());

		VkBuffer buffer;
		VkDeviceMemory buffer_memory;
		VulkanHelper::bufferCreationHelper(
			rhi, static_cast<VkDeviceSize>(m_data.size()),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			buffer, buffer_memory);

		VulkanHelper::moveDataFromVectorToBuffer(rhi, m_data.begin(), m_data.end(), buffer);

		return buffer;
	}
	void Buffer::loadFromgLTF_Buffer(tinygltf::Buffer& buffer) {
		//m_data = std::move(buffer.data);
		m_uri = std::move(buffer.uri);
	}
	bool Buffer::load(const std::string& base_dir,std::string& err,std::string& warn) {
		tinygltf::FsCallbacks fs{ &tinygltf::FileExists, &tinygltf::ExpandFilePath, &tinygltf::ReadWholeFile, &tinygltf::WriteWholeFile, nullptr };
		return tinygltf::LoadExternalFile(&m_data, &err, &warn, m_uri, base_dir, false, 0, 0, &fs);
	}

	//accessor
	void Accessor::loadFromgLTF_Accessor(tinygltf::Accessor& accessor) {
		m_buffer_view = accessor.bufferView;
		m_byte_offset = accessor.byteOffset;
		m_component_type = static_cast<ComponentType>(accessor.componentType);
		m_type = static_cast<Type>(accessor.type);
		m_count = accessor.count;

		m_max = std::move(accessor.maxValues);
		m_min = std::move(accessor.minValues);
	}

	//buffer view
	void BufferView::loadFromgLTF_BufferView(tinygltf::BufferView& buffer_view) {
		m_buffer		= buffer_view.buffer;
		m_byte_length	= buffer_view.byteLength;
		m_byte_offset	= buffer_view.byteOffset;
		m_byte_stride	= buffer_view.byteStride;

		m_target		= static_cast<Target>(buffer_view.target);
	}

	//sampler
	VkSampler Sampler::asVulkanSampler(VulkanRHI* rhi, VkSamplerCreateInfo* optional_info) {
		VkSampler sampler;

		VkSamplerCreateInfo sampler_create_info = VulkanInfo::aboutVkSamplerCreateInfo();
		sampler_create_info.magFilter	= static_cast<VkFilter>(m_magfilter);
		sampler_create_info.minFilter	= static_cast<VkFilter>(m_minfilter);
		sampler_create_info.mipmapMode	= static_cast<VkSamplerMipmapMode>(m_mipmapfilter);

		sampler_create_info.addressModeU = static_cast<VkSamplerAddressMode>(m_address_mode_u);
		sampler_create_info.addressModeV = static_cast<VkSamplerAddressMode>(m_address_mode_v);
		sampler_create_info.addressModeW = static_cast<VkSamplerAddressMode>(m_address_mode_w);

		if (optional_info != nullptr) {
			sampler_create_info.borderColor				= optional_info->borderColor;
			sampler_create_info.anisotropyEnable		= optional_info->anisotropyEnable;
			sampler_create_info.maxAnisotropy			= optional_info->maxAnisotropy;
			sampler_create_info.compareEnable			= optional_info->compareEnable;
			sampler_create_info.compareOp				= optional_info->compareOp;
			sampler_create_info.mipLodBias				= optional_info->mipLodBias;
			sampler_create_info.maxLod					= optional_info->maxLod;
			sampler_create_info.minLod					= optional_info->minLod;
			sampler_create_info.unnormalizedCoordinates = optional_info->unnormalizedCoordinates;
		}

		if (VK_SUCCESS != vkCreateSampler(rhi->m_device, &sampler_create_info, nullptr, &sampler)) {
			MAGE_THROW(failed to create sampler)
		}

		return sampler;
	}
	void Sampler::loadFromgLTF_Sampler(tinygltf::Sampler& gltf_sampler) {
		m_magfilter = static_cast<MageFilter>(gltf_sampler.magFilter - TINYGLTF_TEXTURE_FILTER_NEAREST);

		switch (gltf_sampler.minFilter)
		{
		case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
			m_minfilter = MageFilter::MAGE_FILTER_NEAREST;
			m_mipmapfilter = MageFilter::MAGE_FILTER_NEAREST;
			break;
		case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
			m_minfilter = MageFilter::MAGE_FILTER_NEAREST;
			m_mipmapfilter = MageFilter::MAGE_FILTER_LINEAR;
			break;
		case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
			m_minfilter = MageFilter::MAGE_FILTER_LINEAR;
			m_mipmapfilter = MageFilter::MAGE_FILTER_NEAREST;
			break;
		case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
			m_minfilter = MageFilter::MAGE_FILTER_LINEAR;
			m_mipmapfilter = MageFilter::MAGE_FILTER_LINEAR;
			break;
		}

		switch (gltf_sampler.wrapS) {
		case TINYGLTF_TEXTURE_WRAP_REPEAT:
			m_address_mode_u = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
			m_address_mode_u = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
			m_address_mode_u = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		}

		switch (gltf_sampler.wrapT)
		{
		case TINYGLTF_TEXTURE_WRAP_REPEAT:
			m_address_mode_v = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
			m_address_mode_v = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
			m_address_mode_v = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		}

		m_address_mode_w = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT;
	}

	//texture
	VkRenderTexture Texture::asVulkanRenderTexture(VulkanRHI* rhi) {
		assert(!m_data.empty());

		VkRenderTexture render_texture;

		//create image
		{
			VulkanHelper::imageCreationHelper(
				rhi, m_width, m_height, static_cast<VkFormat>(m_format),
				1,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				render_texture.m_texture, render_texture.m_texture_memory);

			//copy data to staging buffer
			VkBuffer staging_buffer;
			VkDeviceMemory staging_memory;

			VulkanHelper::bufferCreationHelper(
				rhi, m_data.size(),
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				staging_buffer, staging_memory);


			void* copy_to{ nullptr };
			vkMapMemory(rhi->m_device, staging_memory, 0, m_data.size(), 0, &copy_to);
			memcpy(copy_to, m_data.data(), static_cast<size_t>(m_data.size()));
			vkUnmapMemory(rhi->m_device, staging_memory);

			//transition image layout
			VulkanHelper::transitionImageLayout(rhi, render_texture.m_texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipmap_levels);

			//copy data from buffer to image
			VulkanHelper::fromBufferToImageCopyHelper(rhi, staging_buffer, render_texture.m_texture, m_width, m_height);

			//TODO:generate mipmaps

			//transition image layout to shader read only
			VulkanHelper::transitionImageLayout(rhi, render_texture.m_texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipmap_levels);
		
			vkDestroyBuffer(rhi->m_device, staging_buffer, nullptr);
			vkFreeMemory(rhi->m_device, staging_memory, nullptr);
		}

		//create image view
		VkImageViewCreateInfo view_create_info = VulkanInfo::aboutVkImageViewCeateInfo();
		{
			view_create_info.image = render_texture.m_texture;
			view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view_create_info.format = static_cast<VkFormat>(m_format);
			view_create_info.subresourceRange = VulkanInfo::aboutVkImageSubresourceRange();
			view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view_create_info.subresourceRange.baseMipLevel = 0;
			view_create_info.subresourceRange.levelCount = m_mipmap_levels;
			view_create_info.subresourceRange.baseArrayLayer = 0;
			view_create_info.subresourceRange.layerCount = m_layer_counts;
		}
		if (VK_SUCCESS != vkCreateImageView(rhi->m_device, &view_create_info, nullptr, &render_texture.m_texture_view)) {
			MAGE_THROW(failed to create texture image view)
		}

		//create sampler
		render_texture.m_sampler = m_combined_sampler.asVulkanSampler(rhi);

		return render_texture;
	}
	void Texture::loadFromgLTF_Image(tinygltf::Image& gltf_image, tinygltf::Sampler& gltf_sampler) {
		if (gltf_image.bufferView == -1) {
			//TODO:因为我更改了tinygltf库的图片加载部分，现在image中没有图像数据，只有数据文件路径
			//m_data = std::move(gltf_image.image);
			m_uri = std::move(gltf_image.uri);

			m_width = (uint32_t)gltf_image.width;
			m_height = (uint32_t)gltf_image.height;
			m_layer_counts = 1;
			m_mipmap_levels = 1;

			if (gltf_image.component == 4) {
				switch (gltf_image.pixel_type)
				{
				case TINYGLTF_COMPONENT_TYPE_BYTE:
					m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_SNORM;
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_UNORM;
					break;
				case TINYGLTF_COMPONENT_TYPE_INT:
					m_format = MageFormat::MAGE_FORMAT_R32G32B32A32_SINT;
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					m_format = MageFormat::MAGE_FORMAT_R32G32B32A32_UINT;
					break;
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
					m_format = MageFormat::MAGE_FORMAT_R32G32B32A32_SFLOAT;
					break;
				default:
					MAGE_THROW(unsupport format texture)
				}
			}
			else if (gltf_image.component == 3) {
				switch (gltf_image.pixel_type)
				{
				case TINYGLTF_COMPONENT_TYPE_BYTE:
					m_format = MageFormat::MAGE_FORMAT_R8G8B8_SNORM;
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_UNORM;
					break;
				case TINYGLTF_COMPONENT_TYPE_INT:
					m_format = MageFormat::MAGE_FORMAT_R32G32B32_SINT;
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					m_format = MageFormat::MAGE_FORMAT_R32G32B32_UINT;
					break;
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
					m_format = MageFormat::MAGE_FORMAT_R32G32B32_SFLOAT;
					break;
				default:
					MAGE_THROW(unsupport texture format)
				}
			}
			else if (gltf_image.component == 2) {
				switch (gltf_image.pixel_type)
				{
				case TINYGLTF_COMPONENT_TYPE_INT:
					m_format = MageFormat::MAGE_FORMAT_R32G32_SINT;
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					m_format = MageFormat::MAGE_FORMAT_R32G32_UINT;
					break;
				default:
					MAGE_THROW(unsupport texture format)
				}
			}
			else {
				MAGE_THROW(unsupport texture format)
			}

			m_combined_sampler.loadFromgLTF_Sampler(gltf_sampler);
		}
		else {
			//TODO: fetch texture data in buffer.
		}
	}
	bool Texture::load(const std::string& base_dir, std::string& err, std::string& warn) {
		tinygltf::FsCallbacks fs{ &tinygltf::FileExists, &tinygltf::ExpandFilePath, &tinygltf::ReadWholeFile, &tinygltf::WriteWholeFile, nullptr };
		return tinygltf::LoadExternalFile(&m_data, &err, &warn, m_uri, base_dir, false, 0, 0, &fs);
	}

	//Material
	void Material::loadFromgLTF_Material(tinygltf::Material& material) {
		m_emissive_factor[0] = (float)material.emissiveFactor[0];
		m_emissive_factor[1] = (float)material.emissiveFactor[1];
		m_emissive_factor[2] = (float)material.emissiveFactor[2];

		m_pbr_metallic_roughness.m_base_color_factor[0]	= (float)material.pbrMetallicRoughness.baseColorFactor[0];
		m_pbr_metallic_roughness.m_base_color_factor[1]	= (float)material.pbrMetallicRoughness.baseColorFactor[1];
		m_pbr_metallic_roughness.m_base_color_factor[2]	= (float)material.pbrMetallicRoughness.baseColorFactor[2];
		m_pbr_metallic_roughness.m_base_color_factor[3]	= (float)material.pbrMetallicRoughness.baseColorFactor[3];
		m_pbr_metallic_roughness.m_metallic_factor		= (float)material.pbrMetallicRoughness.metallicFactor;
		m_pbr_metallic_roughness.m_roughness_factor		= (float)material.pbrMetallicRoughness.roughnessFactor;
		m_pbr_metallic_roughness.m_base_color_texture.m_index				= material.pbrMetallicRoughness.baseColorTexture.index;
		m_pbr_metallic_roughness.m_base_color_texture.m_texCoord			= material.pbrMetallicRoughness.baseColorTexture.texCoord;
		m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index		= material.pbrMetallicRoughness.metallicRoughnessTexture.index;
		m_pbr_metallic_roughness.m_metallic_roughness_texture.m_texCoord	= material.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;

		m_normal_texture.m_index	= material.normalTexture.index;
		m_normal_texture.m_texCoord = material.normalTexture.texCoord;
		m_normal_texture.m_scale	= material.normalTexture.scale;

		m_emissive_texture.m_index		= material.emissiveTexture.index;
		m_emissive_texture.m_texCoord	= material.emissiveTexture.texCoord;

		m_occlusion_texture.m_index		= material.occlusionTexture.index;
		m_occlusion_texture.m_texCoord	= material.occlusionTexture.texCoord;
		m_occlusion_texture.m_strength	= material.occlusionTexture.strength;
	}

	//primitive
	void Primitive::loadFromgLTF_Primitive(tinygltf::Primitive& primitive) {
		m_attributes = std::move(primitive.attributes);
		m_indices = primitive.indices;
		m_material = primitive.material;
		m_mode = primitive.mode;
	}

	//mesh
	void Mesh::loadFromgLTF_Mesh(tinygltf::Mesh& mesh) {
		m_primitives.resize(mesh.primitives.size());
		for (int i = 0; i < m_primitives.size(); ++i) {
			m_primitives[i].loadFromgLTF_Primitive(mesh.primitives[i]);
		}
	}

	//node
	void Node::loadFromgLTF_Node(tinygltf::Node& node) {
		m_camera = node.camera;
		m_mesh = node.mesh;

		m_children = std::move(node.children);
		//change double to float 
		std::copy(node.rotation.begin(), node.rotation.end(), std::back_inserter(m_rotation));
		std::copy(node.scale.begin(), node.scale.end(), std::back_inserter(m_scale));
		std::copy(node.translation.begin(), node.translation.end(), std::back_inserter(m_translation));
		std::copy(node.matrix.begin(), node.matrix.end(), std::back_inserter(m_matrix));

		//clear raw data
		node.rotation.clear();
		node.scale.clear();
		node.translation.clear();
		node.matrix.clear();

		m_parent = -1;
	}

	//model
	void Model::loadFromgLTF_Model(tinygltf::Model& model) {
		m_meshes.resize(model.meshes.size());
		m_accessors.resize(model.accessors.size());
		m_buffers.resize(model.buffers.size());
		m_buffer_views.resize(model.bufferViews.size());
		m_textures.resize(model.textures.size());
		m_nodes.resize(model.nodes.size());
		m_materials.resize(model.materials.size());

		for (int i = 0; i < m_meshes.size();				++i) m_meshes[i].loadFromgLTF_Mesh(model.meshes[i]);
		for (int i = 0; i < m_accessors.size();				++i) m_accessors[i].loadFromgLTF_Accessor(model.accessors[i]);
		for (int i = 0; i < m_buffers.size();				++i) m_buffers[i].loadFromgLTF_Buffer(model.buffers[i]);
		for (int i = 0; i < m_buffer_views.size();			++i) m_buffer_views[i].loadFromgLTF_BufferView(model.bufferViews[i]);
		for (int i = 0; i < m_textures.size();				++i) m_textures[i].loadFromgLTF_Image(model.images[model.textures[i].source], model.samplers[model.textures[i].sampler]);
		for (int i = 0; i < m_materials.size();				++i) m_materials[i].loadFromgLTF_Material(model.materials[i]);
		for (int i = 0; i < m_nodes.size();					++i) m_nodes[i].loadFromgLTF_Node(model.nodes[i]);
		for (int i = 0; i < m_nodes.size();					++i) {
			for (int j = 0; j < m_nodes[i].m_children.size(); ++j) {
				m_nodes[m_nodes[i].m_children[j]].m_parent = i;
			}
		}
		
		//TODO:process transform matrixs that attached to specific mesh 
	}
	//TODO
	VkRenderModelInfo Model::getVkRenderModelInfo() {
		VkRenderModelInfo render_model_info;
		render_model_info.m_go_id = m_go_id;

		auto& mesh_info = render_model_info.m_mesh_info;
		auto& material_info = render_model_info.m_material_info;

		assert(m_buffers.size() == 1); //暂不支持多buffer
		mesh_info.m_mesh_uri.m_uri = std::move(m_buffers.front().m_uri);

		int sub_meshes_num{ 0 };
		for (auto& mesh : m_meshes)
			sub_meshes_num += mesh.m_primitives.size();
		mesh_info.m_transfer_mesh_descriptions.resize(sub_meshes_num);
		sub_meshes_num = 0;
		auto mesh_process = [&](const int& mesh_index, const Matrix4x4& matrix)->void {
			for (auto& primitive : m_meshes[mesh_index].m_primitives) {
				; //TODO:目前是排除非index mesh情况，之后需要增加没有indices的情况
				Accessor* index_accessor = nullptr;
				Material* primitive_material = nullptr;

				if (primitive.m_attributes.find("indices") != primitive.m_attributes.end()) {
					index_accessor = &m_accessors[primitive.m_attributes["indices"]];
					assert(index_accessor->m_buffer_view != -1);
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_index_offset	= index_accessor->m_byte_offset + m_buffer_views[index_accessor->m_buffer_view].m_byte_offset;
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_index_count	= index_accessor->m_count;
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_matrix			= matrix;
				}
				else {
					//TODO:非索引mesh，仅有顶点数据
				}

				if (primitive.m_material != -1) {
					primitive_material = &m_materials[primitive.m_material];
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_base_color_factor	= primitive_material->m_pbr_metallic_roughness.m_base_color_factor;
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_metallic_factor	= primitive_material->m_pbr_metallic_roughness.m_metallic_factor;
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_roughness_factor	= primitive_material->m_pbr_metallic_roughness.m_roughness_factor;

					material_info.m_material_uri.m_base_color_uri			= std::move(m_textures[primitive_material->m_pbr_metallic_roughness.m_base_color_texture.m_index].m_uri);
					material_info.m_material_uri.m_metallic_roughness_uri	= std::move(m_textures[primitive_material->m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index].m_uri);
					material_info.m_material_uri.m_normal_uri				= std::move(m_textures[primitive_material->m_normal_texture.m_index].m_uri);

					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_textures_use_info |= material_info.m_material_uri.m_base_color_uri.empty() ? VkRenderMeshDescription::MESH_NO_USE : VkRenderMeshDescription::MESH_USE_BASECOLOR_TEXUTRE;
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_textures_use_info |= material_info.m_material_uri.m_metallic_roughness_uri.empty() ? VkRenderMeshDescription::MESH_NO_USE : VkRenderMeshDescription::MESH_USE_METALLICROUGHNESS_TEXTURE;
					mesh_info.m_transfer_mesh_descriptions[sub_meshes_num].m_textures_use_info |= material_info.m_material_uri.m_normal_uri.empty() ? VkRenderMeshDescription::MESH_NO_USE : VkRenderMeshDescription::MESH_USE_NORMAL_TEXTURE;
				}
				else {
					//TODO:无material的primitive
				}
				++sub_meshes_num;
			}
		};
		Matrix4x4 root_mat{ Matrix4x4::identity };
		processNode(m_nodes, 0, root_mat, std::function<void(const int&, const Matrix4x4&)>(mesh_process));

		return render_model_info;
	}
	void Model::processNode(const std::vector<Node>& nodes, int curr_index, const Matrix4x4& parent_matrix, const std::function<void(const int&, const Matrix4x4&)>& process_func) {
		auto& node = nodes[curr_index];

		//get local transform
		Matrix4x4 curr_matrix{ Matrix4x4::identity };
		if (node.m_matrix.size()) {
			curr_matrix.SetMatrix(node.m_matrix);
		}
		else if (node.m_translation.size() && node.m_rotation.size() && node.m_scale.size()) {
			curr_matrix.SetTRS(Vector3(node.m_translation), Quaternion(node.m_rotation), Vector3(node.m_scale));
		}

		//process node
		if (node.m_mesh != -1) {
			process_func(node.m_mesh, parent_matrix * curr_matrix);
		}
		//TODO:process other things

		//process children
		for (auto& child : node.m_children) {
			processNode(nodes, child, curr_matrix, process_func);
		}
	}

}