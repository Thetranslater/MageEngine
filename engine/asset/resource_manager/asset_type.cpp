#include<assert.h>

#include<engine_core/render_engine/renderer/vulkanInfo.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>

#include<asset/resource_manager/asset_type.h>
#include<engine_core/platform/file_system.h>


namespace Mage {
	//buffer
	VkBuffer Buffer::asVulkanBuffer(VulkanRHI* rhi) {
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
		m_data = std::move(buffer.data);
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
	bool Texture::loadTextureData(const std::string& base_dir) {

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
		m_buffer_relative_uris.resize(model.buffers.size());
		m_buffer_views.resize(model.bufferViews.size());
		m_textures.resize(model.textures.size());
		m_nodes.resize(model.nodes.size());

		for (int i = 0; i < m_meshes.size();				++i) m_meshes[i].loadFromgLTF_Mesh(model.meshes[i]);
		for (int i = 0; i < m_accessors.size();				++i) m_accessors[i].loadFromgLTF_Accessor(model.accessors[i]);
		for (int i = 0; i < m_buffer_relative_uris.size();	++i) m_buffer_relative_uris[i] = std::move(model.buffers[i].uri);
		for (int i = 0; i < m_buffer_views.size();			++i) m_buffer_views[i].loadFromgLTF_BufferView(model.bufferViews[i]);
		for (int i = 0; i < m_textures.size();				++i) m_textures[i].loadFromgLTF_Image(model.images[model.textures[i].source], model.samplers[model.textures[i].sampler]);
		for (int i = 0; i < m_nodes.size();					++i) m_nodes[i].loadFromgLTF_Node(model.nodes[i]);
		for (int i = 0; i < m_nodes.size();					++i) {
			for (int j = 0; j < m_nodes[i].m_children.size(); ++j) {
				m_nodes[m_nodes[i].m_children[j]].m_parent = i;
			}
		}
		
	}
	//TODO:NONE FINISH!!!
	VkRenderModel Model::asVulkanRenderModel(VulkanRHI* rhi) {
		VkRenderModel render_model;
		//假定buffer所有数据紧密排列，顶点数据按照顺序排列，索引数据组合成一个buffer view，所有属性数据挨在一起。
		int index_view_map{ -1 };
		std::array<int, 6> binding_view_map{ -1,-1,-1,-1,-1,-1 };

		//TODO：因为有很多假定情况，所以泛用性不强，有待升级，比如更加混乱的情况，应该将数据首先聚合起来，在复制到vulkan中。
		//在假定情况下，以第一个primitive为基础通过accessor及buffer view查询各数据对应的buffer
		auto& guide_primitive = m_meshes[0].m_primitives[0];
		index_view_map = m_accessors[guide_primitive.m_indices].m_buffer_view;

		auto check_attribute = [&](const std::string& name, int binding/*与顶点属性的binding对应*/) {
			if (guide_primitive.m_attributes.count(name)) {
				binding_view_map[binding] = m_accessors[guide_primitive.m_attributes[name]].m_buffer_view;
			}
		};
		check_attribute("POSITION", 0);
		check_attribute("NORMAL", 1);
		check_attribute("TANGENT", 2);
		check_attribute("TEXCOORD_0", 3);
		check_attribute("TEXCOORD_1", 4);
		check_attribute("COLOR_0", 5);

		//index buffer
		auto& index_buffer_view = m_buffer_views[index_view_map];
		VulkanHelper::bufferCreationHelper(rhi, index_buffer_view.m_byte_length, 
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			render_model.m_index_buffer,render_model.m_index_memory);

		auto index_data_move_start = m_buffers[index_buffer_view.m_buffer].data() + index_buffer_view.m_byte_offset;
		VulkanHelper::moveDataFromVectorToBuffer<unsigned char*>(rhi, index_data_move_start, index_data_move_start + index_buffer_view.m_byte_length, render_model.m_index_buffer);

		//vertex buffer
		for (int i = 0; i < binding_view_map.size(); ++i) {
			if (binding_view_map[i] != -1) {
				//have attribute data
				auto& attribute_buffer_view = m_buffer_views[binding_view_map[i]];
				VulkanHelper::bufferCreationHelper(rhi, attribute_buffer_view.m_byte_length,
					VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					render_model.m_vertex_buffers[i], render_model.m_vertex_memories[i]);

				auto attribute_data_move_start = m_buffers[attribute_buffer_view.m_buffer].data() + attribute_buffer_view.m_byte_offset;
				VulkanHelper::moveDataFromVectorToBuffer<unsigned char*>(rhi, attribute_data_move_start, attribute_data_move_start + attribute_buffer_view.m_byte_length, render_model.m_vertex_buffers[i]);
			}
			else {
				render_model.m_vertex_buffers[i] = VK_NULL_HANDLE;
				render_model.m_vertex_memories[i] = VK_NULL_HANDLE;
			}
		}

		//render texture
		render_model.m_p_textures.resize(m_textures.size());
		for (int i = 0; i < render_model.m_p_textures.size(); ++i) {
			render_model.m_p_textures[i] = std::make_shared<Mage::VkRenderTexture>(m_textures[i].asVulkanRenderTexture(rhi));
		}
		
		//textures' descriptor sets
		VkDescriptorSetLayoutCreateInfo textures_layout_create_info = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
		//TODO:binding count may be increase
		textures_layout_create_info.bindingCount = 3;
		//TODO
		std::array<VkDescriptorSetLayoutBinding, 3> bindings;
		{
			bindings[0] = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			bindings[0].binding = 0; //albedo
			bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			bindings[0].descriptorCount = 1;
			bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			bindings[0].pImmutableSamplers = nullptr;

			bindings[1] = bindings[0];
			bindings[1].binding = 1;

			bindings[2] = bindings[0];
			bindings[2].binding = 2;
		}
		textures_layout_create_info.pBindings = bindings.data();

		VkDescriptorSetLayout textures_layout;
		if (VK_SUCCESS != vkCreateDescriptorSetLayout(rhi->m_device, &textures_layout_create_info, nullptr, &textures_layout)) {
			MAGE_THROW(failed to create textures layout)
		}

		VkDescriptorSetAllocateInfo textures_set_alloc_info = VulkanInfo::aboutVkDescriptorSetAllocateInfo();
		textures_set_alloc_info.descriptorPool = rhi->m_descriptor_pool;
		textures_set_alloc_info.descriptorSetCount = 1;
		textures_set_alloc_info.pSetLayouts = &textures_layout;
		if (VK_SUCCESS != vkAllocateDescriptorSets(rhi->m_device, &textures_set_alloc_info, &render_model.m_texture_descriptor_sets)) {
			MAGE_THROW(failed to create model descriptor set)
		}

		return render_model;
	}

}