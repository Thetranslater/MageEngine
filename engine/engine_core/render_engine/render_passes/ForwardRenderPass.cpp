#include"engine_core/render_engine/render_passes/ForwardRenderPass.h"

#include"engine_core/render_engine/renderer/vulkanInfo.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"

#include"engine_core/render_engine/render_mesh.h"
#include"engine_core/render_engine/render_resource.h"

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
		m_descriptor_sets.layout_infos.resize(2);
		//set 1:global resources, lights, camera, sky
		{
			//TODO
			VkDescriptorSetLayoutBinding global_perframe_layout_binding = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			global_perframe_layout_binding.binding = 0;
			global_perframe_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perframe_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutBinding global_perdrawcall_layout_binding = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			global_perdrawcall_layout_binding.binding = 1;
			global_perdrawcall_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perdrawcall_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			std::array<VkDescriptorSetLayoutBinding, 2> global_setlayout = { global_perframe_layout_binding,global_perdrawcall_layout_binding };

			VkDescriptorSetLayoutCreateInfo set_1 = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
			set_1.bindingCount = 2;
			set_1.pBindings = global_setlayout.data();
			if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device, &set_1, nullptr, &m_descriptor_sets.layout_infos[0])) {
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
			if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device, &set_2, nullptr, &m_descriptor_sets.layout_infos[1])) {
				MAGE_THROW(failed to create forward renderpass descriptor set layout)
			}
		}
	}

	void ForwardRenderPass::setupDescriptorSets() {
		m_descriptor_sets.sets.resize(2);
		//set_1 create
		{
			VkDescriptorSetAllocateInfo global_descriptors_allocate_info = VulkanInfo::aboutVkDescriptorSetAllocateInfo();
			global_descriptors_allocate_info.descriptorPool = m_vulkan_rhi->m_descriptor_pool;
			global_descriptors_allocate_info.descriptorSetCount = 1;
			global_descriptors_allocate_info.pSetLayouts = &m_descriptor_sets.layout_infos[0];

			if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_rhi->m_device,&global_descriptors_allocate_info,&m_descriptor_sets.sets[0])) {
				MAGE_THROW(failde to allocate descriptor sets of forward renderpass)
			}

			VkDescriptorBufferInfo global_buffer_perframe_update_info = VulkanInfo::aboutVkDescriptorBufferInfo();
			global_buffer_perframe_update_info.buffer = m_global_buffer->m_buffer;
			global_buffer_perframe_update_info.offset = 0;
			//TODO:需要定义binding buffer的大小
			global_buffer_perframe_update_info.range = -1;//TODO

			VkWriteDescriptorSet global_perframe_descriptor_write = VulkanInfo::aboutVkWriteDescriptorSet();
			global_perframe_descriptor_write.dstSet = m_descriptor_sets.sets[0];
			global_perframe_descriptor_write.dstBinding = 0;
			global_perframe_descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perframe_descriptor_write.pBufferInfo = &global_buffer_perframe_update_info;

			VkDescriptorBufferInfo global_buffer_perdrawcall_update_info = VulkanInfo::aboutVkDescriptorBufferInfo();
			global_buffer_perdrawcall_update_info.buffer = m_global_buffer->m_buffer;
			global_buffer_perdrawcall_update_info.offset = 0;
			//TODO:需要定义binding buffer的大小
			global_buffer_perdrawcall_update_info.range = -1;//TODO

			VkWriteDescriptorSet global_perdrawcall_descriptor_write = VulkanInfo::aboutVkWriteDescriptorSet();
			global_perdrawcall_descriptor_write.dstSet = m_descriptor_sets.sets[0];
			global_perdrawcall_descriptor_write.dstBinding = 1;
			global_perdrawcall_descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perdrawcall_descriptor_write.pBufferInfo = &global_buffer_perdrawcall_update_info;
		}
		//set_2 create
		{

		}
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