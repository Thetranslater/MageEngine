#include"engine_core/render_engine/render_passes/ForwardRenderPass.h"

#include"engine_core/render_engine/renderer/vulkanInfo.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"

#include"engine_core/render_engine/render_mesh.h"

namespace Mage {
	void ForwardRenderPass::initialize(const RenderPassCreateInfo* createInfo) {
		RenderPass::initialize(createInfo);

		setupDescriptorLayouts();
		setupDescriptorSets();
		setupRenderPass();
		setupSubPasses();
		setupFramebuffers();
	}

	void ForwardRenderPass::setupDescriptorLayouts() {
		m_descriptor_sets.m_descriptor_set_layouts.resize(2);
		//set 1:global resources, lights, camera, sky
		{
			//TODO
			VkDescriptorSetLayoutCreateInfo set_1 = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
			set_1.bindingCount = 0;
			set_1.pBindings = nullptr;
			if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device, &set_1, nullptr, &m_descriptor_sets.m_descriptor_set_layouts[0])) {
				MAGE_THROW(failed to create forward renderpass descriptor set layout)
			}
		}
		//set 2: custom textures
		{
			VkDescriptorSetLayoutCreateInfo set_2 = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
			set_2.bindingCount = 3;
			//bindings
			std::array<VkDescriptorSetLayoutBinding, 3> set_2_bindings;
			{
				//albedo
				set_2_bindings[0] = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
				set_2_bindings[0].binding = 0;
				set_2_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				set_2_bindings[0].descriptorCount = 1;
				set_2_bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				set_2_bindings[0].pImmutableSamplers = nullptr;
				//metallic
				set_2_bindings[1] = set_2_bindings[0];
				set_2_bindings[1].binding = 1;
				//roughness
				set_2_bindings[2] = set_2_bindings[0];
				set_2_bindings[2].binding = 2;
			}
			set_2.pBindings = set_2_bindings.data();
			if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device, &set_2, nullptr, &m_descriptor_sets.m_descriptor_set_layouts[1])) {
				MAGE_THROW(failed to create forward renderpass descriptor set layout)
			}
		}
	}

	void ForwardRenderPass::setupDescriptorSets() {
		//set_1 create
	}

	void ForwardRenderPass::setupRenderPass() {
		VkRenderPassCreateInfo create_info = VulkanInfo::aboutVkRenderPassCreateInfo();

		m_attachments.m_attachment_descriptions.resize(2);
		m_attachments.m_image_views.resize(2);
		{
			m_attachments.m_attachment_descriptions[0] = VulkanInfo::aboutVkAttachmentDescription();
			m_attachments.m_attachment_descriptions[0].format = m_vulkan_rhi->m_swapchain_surface_format.format;
			m_attachments.m_attachment_descriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			m_attachments.m_attachment_descriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			m_attachments.m_attachment_descriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			m_attachments.m_attachment_descriptions[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			//TODO:depth attachment
			m_attachments.m_attachment_descriptions[1] = VulkanInfo::aboutVkAttachmentDescription();

		}
	}
}