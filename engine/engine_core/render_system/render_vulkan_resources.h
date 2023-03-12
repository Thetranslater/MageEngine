#pragma once
#include<vulkan/vulkan.h>

#include<vector>

namespace Mage {

	struct DescriptorSets
	{
		std::vector<VkDescriptorSetLayout> layout_infos;
		std::vector<VkDescriptorSet> sets;
	};

	struct Attachments
	{
		std::vector<VkImageView> m_image_views;							//for framebuffer
		std::vector<VkAttachmentDescription> m_attachment_descriptions;	//for renderpass

		std::vector<VkImage> m_images;				//for physical attachment
		std::vector<VkDeviceMemory> m_image_memories;	//for physical attachment
	};
}
