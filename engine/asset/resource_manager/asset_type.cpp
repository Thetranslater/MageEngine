#include<engine_core/render_engine/renderer/vulkanInfo.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>

#include<asset/resource_manager/asset_type.h>


namespace Mage {
	VkImage Texture::asVulkanImage(VulkanRHI* rhi) {
		VkImage image;
		VkDeviceMemory image_memory;

		//create image
		VulkanHelper::imageCreationHelper(
			rhi, m_width, m_height, static_cast<VkFormat>(m_format),
			1,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			image, image_memory);

		//copy data to staging buffer
		VkBuffer staging_buffer;
		VkDeviceMemory staging_memory;

		VulkanHelper::bufferCreationHelper(
			rhi, m_size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			staging_buffer, staging_memory);


		void* copy_to{ nullptr };
		vkMapMemory(rhi->m_device, staging_memory, 0, m_size, 0, &copy_to);
		memcpy(copy_to, m_data, static_cast<size_t>(m_size));
		vkUnmapMemory(rhi->m_device, staging_memory);

		//transition image layout
		VulkanHelper::transitionImageLayout(rhi, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipmap_levels);

		//copy data from buffer to image
		VulkanHelper::fromBufferToImageCopyHelper(rhi, staging_buffer, image, m_width, m_height);

		//transition image layout to shader read only
		VulkanHelper::transitionImageLayout(rhi, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipmap_levels);

		return image;
	}
}