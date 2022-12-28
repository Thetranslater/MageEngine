#include<engine_core/render_engine/render_resource.h>
#include<engine_core/render_engine/renderer/vulkanInfo.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>

#include<asset/resource_manager/asset_type.h>

namespace Mage {
	void RenderResource::initialize(VulkanRHI* rhi) {
		//create global buffer;
		VkDeviceSize global_buffer_size = 1024 * 1024 * 20;

		//resize
		int size = rhi->m_swapchain_size;
		m_global_updated_buffer.m_buffers.resize(size);
		m_global_updated_buffer.m_buffer_memories.resize(size);
		m_global_updated_buffer.m_followed_camera_updated_data_pointers.resize(size);

		for (int i{ 0 }; i < size; ++i) {
			VulkanHelper::bufferCreationHelper(rhi, global_buffer_size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_global_updated_buffer.m_buffers[i], m_global_updated_buffer.m_buffer_memories[i]);
			//TODO:unmap as program terminated.
			vkMapMemory(rhi->m_device, m_global_updated_buffer.m_buffer_memories[i], 0, VK_WHOLE_SIZE, 0, &m_global_updated_buffer.m_followed_camera_updated_data_pointers[i]);
		}
	}

	bool RenderResource::hasMesh(const GUID32& guid) {
		return m_guid_buffer_map.find(guid) != m_guid_buffer_map.end();
	}

	bool RenderResource::hasTexture(const GUID32& guid) {
		return m_guid_texture_map.find(guid) != m_guid_texture_map.end();
	}

	bool RenderResource::hasMaterial(const GUID64& guid) {
		return m_guid_material_map.find(guid) != m_guid_material_map.end();
	}

	bool RenderResource::getOrCreateRenderResource(VulkanRHI* rhi, GUID32& guid, IO_Buffer& buffer) {
		if (hasMesh(guid)) {
			buffer = m_guid_buffer_map[guid];
			return true;
		}

		assert(not std::get<1>(buffer).m_data.empty());

		VkRenderMesh render_buffer{};
		auto& data = std::get<1>(buffer).m_data;
		//create buffer and bind memory before insert to map
		VulkanHelper::bufferCreationHelper(rhi, data.size(),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			render_buffer.m_bi_data, render_buffer.m_bi_data_memory);

		VulkanHelper::moveDataFromVectorToBuffer(rhi, data.begin(), data.end(), render_buffer.m_bi_data);
		data.clear();
		buffer = render_buffer;
		m_guid_buffer_map.emplace(std::make_pair(guid, render_buffer));
		return true;
	}

	bool RenderResource::getOrCreateRenderResource(VulkanRHI* rhi, GUID32& guid, IO_Texture& texture) {
		if (hasTexture(guid)) {
			texture = m_guid_texture_map[guid];
			return true;
		}

		assert(not std::get<1>(texture).m_data.empty());

		VkRenderTexture render_texture;
		auto& itexture = std::get<1>(texture);
		VkDeviceSize texture_size = itexture.m_data.size();

		//create image and bind memory before insert to map
		VulkanHelper::imageCreationHelper(rhi, itexture.m_width, itexture.m_height,
			static_cast<VkFormat>(itexture.m_format), 1,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			render_texture.m_texture, render_texture.m_texture_memory);

		//create temporary buffer to store the image data
		VkBuffer staging_buffer;
		VkDeviceMemory staging_memory;
		VulkanHelper::bufferCreationHelper(rhi, texture_size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			staging_buffer, staging_memory);

		//copy data from out to vkbuffer
		void* ppdata{ nullptr };
		vkMapMemory(rhi->m_device, staging_memory, 0, texture_size, 0, &ppdata);
		memcpy(ppdata, itexture.m_data.data(), texture_size);
		vkUnmapMemory(rhi->m_device, staging_memory);

		//transition image layout to destination
		VulkanHelper::transitionImageLayout(rhi, render_texture.m_texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);

		//copy image from vkbuffer to vkimage
		VulkanHelper::fromBufferToImageCopyHelper(rhi, staging_buffer, render_texture.m_texture, itexture.m_width, itexture.m_height);

		//generate mipmaps

		//transition image layout to shader read only.
		//we can put this process into mipmaps generating function 
		VulkanHelper::transitionImageLayout(rhi, render_texture.m_texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

		//image view creation
		VkImageViewCreateInfo image_view_creatInfo = VulkanInfo::aboutVkImageViewCeateInfo();
		image_view_creatInfo.image = render_texture.m_texture;
		image_view_creatInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_creatInfo.format = static_cast<VkFormat>(itexture.m_format);
		image_view_creatInfo.subresourceRange = VulkanInfo::aboutVkImageSubresourceRange();
		image_view_creatInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_creatInfo.subresourceRange.baseArrayLayer = 0;
		image_view_creatInfo.subresourceRange.baseMipLevel = 0;
		image_view_creatInfo.subresourceRange.layerCount = 1;
		image_view_creatInfo.subresourceRange.levelCount = 1;

		if (VK_SUCCESS != vkCreateImageView(rhi->m_device, &image_view_creatInfo, nullptr, &render_texture.m_texture_view)) {
			MAGE_THROW(failed to create image view for asset)
		}

		//smapler creation
		render_texture.m_sampler = itexture.m_combined_sampler.asVulkanSampler(rhi);

		//free temporary buffer
		vkDestroyBuffer(rhi->m_device, staging_buffer, nullptr);
		vkFreeMemory(rhi->m_device, staging_memory, nullptr);

		m_guid_texture_map.emplace(std::make_pair(guid, render_texture));

		return true;
	}

	bool RenderResource::getOrCreateRenderResource(VulkanRHI* rhi, GUID64& guid, IO_Material& material) {
		if (hasMaterial(guid)) {
			material = m_guid_material_map[guid];
			return true;
		}

		VkRenderMaterial render_material;
		render_material.m_double_side = std::get<1>(material).m_double_side;

		//TODO:Ôö¼Óocculusion texture
		VkDescriptorSetLayoutBinding bindings[3];
		bindings[0] = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
		bindings[0].binding = 0;
		bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[0].pImmutableSamplers = std::get<1>(material).m_base_color_texture_index != 0xffffffff ?
			&m_guid_texture_map[std::get<1>(material).m_base_color_texture_index].m_sampler : nullptr;
		bindings[1] = bindings[0];
		bindings[1].binding = 1;
		bindings[1].pImmutableSamplers = std::get<1>(material).m_normal_texture_index != 0xfffffff ?
			&m_guid_texture_map[std::get<1>(material).m_normal_texture_index].m_sampler : nullptr;
		bindings[2] = bindings[0];
		bindings[2].binding = 2;
		bindings[2].pImmutableSamplers = std::get<1>(material).m_metallic_roughness_texture_index != 0xffffffff ?
			&m_guid_texture_map[std::get<1>(material).m_metallic_roughness_texture_index].m_sampler : nullptr;

		VkDescriptorSetLayoutCreateInfo layout_info = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
		layout_info.bindingCount = 3;
		layout_info.pBindings = bindings;
		VkDescriptorSetLayout layout;
		if (VK_SUCCESS != vkCreateDescriptorSetLayout(rhi->m_device, &layout_info, nullptr, &layout)) {
			MAGE_THROW(failed to create descriptor set layout in material)
		}

		VkDescriptorSetAllocateInfo alloc_info = VulkanInfo::aboutVkDescriptorSetAllocateInfo();
		alloc_info.descriptorPool = rhi->m_descriptor_pool;
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = &layout;

		if (VK_SUCCESS != vkAllocateDescriptorSets(rhi->m_device, &alloc_info, &render_material.m_descriptor_set)) {
			MAGE_THROW(failed to allocate descritor set when create material resource)
		}


		VkDescriptorImageInfo binding_base_color = VulkanInfo::aboutVkDescriptorImageInfo();
		binding_base_color.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		binding_base_color.imageView = m_guid_texture_map[std::get<1>(material).m_base_color_texture_index].m_texture_view;
		VkWriteDescriptorSet material_write_binding_0 = VulkanInfo::aboutVkWriteDescriptorSet();
		material_write_binding_0.dstSet = render_material.m_descriptor_set;
		material_write_binding_0.dstBinding = 0;
		material_write_binding_0.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		material_write_binding_0.pImageInfo = &binding_base_color;

		VkDescriptorImageInfo binding_normal = VulkanInfo::aboutVkDescriptorImageInfo();
		binding_normal = binding_base_color;
		binding_normal.imageView = m_guid_texture_map[std::get<1>(material).m_normal_texture_index].m_texture_view;
		VkWriteDescriptorSet material_write_binding_1 = VulkanInfo::aboutVkWriteDescriptorSet();
		material_write_binding_1 = material_write_binding_0;
		material_write_binding_1.dstBinding = 1;
		material_write_binding_1.pImageInfo = &binding_normal;

		VkDescriptorImageInfo binding_metallic_roughness = VulkanInfo::aboutVkDescriptorImageInfo();
		binding_metallic_roughness = binding_base_color;
		binding_metallic_roughness.imageView = m_guid_texture_map[std::get<1>(material).m_metallic_roughness_texture_index].m_texture_view;
		VkWriteDescriptorSet material_write_binding_2 = VulkanInfo::aboutVkWriteDescriptorSet();
		material_write_binding_2 = material_write_binding_0;
		material_write_binding_2.dstBinding = 2;
		material_write_binding_2.pImageInfo = &binding_metallic_roughness;

		VkWriteDescriptorSet updates[3] = { material_write_binding_0,material_write_binding_1,material_write_binding_2 };
		vkUpdateDescriptorSets(rhi->m_device, 3, updates, 0, nullptr);

		m_guid_material_map.insert(std::make_pair(guid, render_material));

		return true;
	}
}