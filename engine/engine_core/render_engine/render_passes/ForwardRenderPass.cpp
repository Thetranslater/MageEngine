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
			global_perframe_layout_binding.binding				= 0;
			global_perframe_layout_binding.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perframe_layout_binding.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutBinding global_perdrawcall_layout_binding = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			global_perdrawcall_layout_binding.binding			= 1;
			global_perdrawcall_layout_binding.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perdrawcall_layout_binding.stageFlags		= VK_SHADER_STAGE_VERTEX_BIT;

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
				set_2_bindings[0]						= VulkanInfo::aboutVkDescriptorSetLayoutBinding();
				set_2_bindings[0].binding				= 0;
				set_2_bindings[0].descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				set_2_bindings[0].descriptorCount		= 1;
				set_2_bindings[0].stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT;
				set_2_bindings[0].pImmutableSamplers	= nullptr;
				//metallic
				set_2_bindings[1]			= set_2_bindings[0];
				set_2_bindings[1].binding	= 1;
				//roughness
				set_2_bindings[2]			= set_2_bindings[0];
				set_2_bindings[2].binding	= 2;
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
			VkDescriptorSetAllocateInfo global_descriptors_allocate_info	= VulkanInfo::aboutVkDescriptorSetAllocateInfo();
			global_descriptors_allocate_info.descriptorPool		= m_vulkan_rhi->m_descriptor_pool;
			global_descriptors_allocate_info.descriptorSetCount	= 1;
			global_descriptors_allocate_info.pSetLayouts		= &m_descriptor_sets.layout_infos[0];

			if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_rhi->m_device,&global_descriptors_allocate_info,&m_descriptor_sets.sets[0])) {
				MAGE_THROW(failde to allocate descriptor sets of forward renderpass)
			}

			VkDescriptorBufferInfo global_buffer_perframe_update_info	= VulkanInfo::aboutVkDescriptorBufferInfo();
			global_buffer_perframe_update_info.buffer			= m_global_buffer->m_buffer;
			global_buffer_perframe_update_info.offset			= 0;
			//TODO:需要定义binding buffer的大小
			global_buffer_perframe_update_info.range			= -1;//TODO

			VkWriteDescriptorSet global_perframe_descriptor_write	= VulkanInfo::aboutVkWriteDescriptorSet();
			global_perframe_descriptor_write.dstSet				= m_descriptor_sets.sets[0];
			global_perframe_descriptor_write.dstBinding			= 0;
			global_perframe_descriptor_write.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perframe_descriptor_write.pBufferInfo		= &global_buffer_perframe_update_info;

			VkDescriptorBufferInfo global_buffer_perdrawcall_update_info	= VulkanInfo::aboutVkDescriptorBufferInfo();
			global_buffer_perdrawcall_update_info.buffer		= m_global_buffer->m_buffer;
			global_buffer_perdrawcall_update_info.offset		= 0;
			//TODO:需要定义binding buffer的大小
			global_buffer_perdrawcall_update_info.range			= -1;//TODO

			VkWriteDescriptorSet global_perdrawcall_descriptor_write	= VulkanInfo::aboutVkWriteDescriptorSet();
			global_perdrawcall_descriptor_write.dstSet			= m_descriptor_sets.sets[0];
			global_perdrawcall_descriptor_write.dstBinding		= 1;
			global_perdrawcall_descriptor_write.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perdrawcall_descriptor_write.pBufferInfo		= &global_buffer_perdrawcall_update_info;

			std::array<VkWriteDescriptorSet, 2> writes = { global_perframe_descriptor_write, global_perdrawcall_descriptor_write };

			vkUpdateDescriptorSets(m_vulkan_rhi->m_device, 2, writes.data(), 0, nullptr);
		}
		//TODO:set_2 create
		{

		}
	}

	void ForwardRenderPass::setupRenderPass() {
		VkRenderPassCreateInfo create_info = VulkanInfo::aboutVkRenderPassCreateInfo();

		m_attachments.m_attachment_descriptions.resize(2);
		m_attachments.m_image_views.resize(2);
		{
			//swapchain
			m_attachments.m_attachment_descriptions[0]					= VulkanInfo::aboutVkAttachmentDescription();
			m_attachments.m_attachment_descriptions[0].format			= m_vulkan_rhi->m_swapchain_surface_format.format;
			m_attachments.m_attachment_descriptions[0].loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
			m_attachments.m_attachment_descriptions[0].storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
			m_attachments.m_attachment_descriptions[0].stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[0].stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[0].initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
			m_attachments.m_attachment_descriptions[0].finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			//depth attachment
			m_attachments.m_attachment_descriptions[1]					= VulkanInfo::aboutVkAttachmentDescription();
			m_attachments.m_attachment_descriptions[1].format			= m_vulkan_rhi->m_depth_format;
			m_attachments.m_attachment_descriptions[1].loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
			m_attachments.m_attachment_descriptions[1].storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
			m_attachments.m_attachment_descriptions[1].stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[1].stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[1].initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
			m_attachments.m_attachment_descriptions[1].finalLayout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		m_p_subpasses.resize(1);
		VkSubpassDescription directional_lighting_subpass_desc = VulkanInfo::aboutVkSubpassDescription();
		{
			directional_lighting_subpass_desc.pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;
			directional_lighting_subpass_desc.inputAttachmentCount		= 0;
			directional_lighting_subpass_desc.pInputAttachments			= nullptr;
			directional_lighting_subpass_desc.colorAttachmentCount		= 1;
			VkAttachmentReference color_attachment_ref = VulkanInfo::aboutVkAttachmentReference();
			color_attachment_ref.attachment								= 0;
			color_attachment_ref.layout									= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			directional_lighting_subpass_desc.pColorAttachments			= &color_attachment_ref;
			directional_lighting_subpass_desc.pResolveAttachments		= nullptr;
			VkAttachmentReference depth_attachment_ref = VulkanInfo::aboutVkAttachmentReference();
			depth_attachment_ref.attachment								= 1;
			depth_attachment_ref.layout									= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			directional_lighting_subpass_desc.pDepthStencilAttachment	= &depth_attachment_ref;
			directional_lighting_subpass_desc.preserveAttachmentCount	= 0;
			directional_lighting_subpass_desc.pPreserveAttachments		= nullptr;
		}

		VkSubpassDependency directional_lighting_subpass_depend = VulkanInfo::aboutVkSubpassDependency();
		{
			directional_lighting_subpass_depend.srcSubpass		= VK_SUBPASS_EXTERNAL;
			directional_lighting_subpass_depend.dstSubpass		= 0;
			directional_lighting_subpass_depend.srcStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			directional_lighting_subpass_depend.srcAccessMask	= 0;
			directional_lighting_subpass_depend.dstStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			directional_lighting_subpass_depend.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}
		
		create_info.attachmentCount = 2;
		create_info.pAttachments	= m_attachments.m_attachment_descriptions.data();
		create_info.subpassCount	= 1;
		create_info.pSubpasses		= &directional_lighting_subpass_desc;
		create_info.dependencyCount = 1;
		create_info.pDependencies	= &directional_lighting_subpass_depend;

		if (VK_SUCCESS != vkCreateRenderPass(m_vulkan_rhi->m_device, &create_info, nullptr, &m_render_pass)) {
			MAGE_THROW(faild to create directional lighting renderpass)
		}
	}

	void ForwardRenderPass::setupFramebuffers() {
		m_framebuffers.resize(m_vulkan_rhi->m_swapchain_size);
		//global attachments
		{
			m_attachments.m_image_views[1] = m_vulkan_rhi->m_depth_image_view;
		}
		for (int i{ 0 }; i < m_framebuffers.size(); ++i) {
			//dynamic attachments
			{
				m_attachments.m_image_views[0] = m_vulkan_rhi->m_swapchain_image_views[i];
			}

			VkFramebufferCreateInfo framebuffer_create_info = VulkanInfo::aboutVkFramebufferCreateInfo();
			framebuffer_create_info.renderPass		= m_render_pass;
			framebuffer_create_info.attachmentCount	= static_cast<uint32_t>(m_attachments.m_image_views.size());
			framebuffer_create_info.pAttachments	= m_attachments.m_image_views.data();
			framebuffer_create_info.width			= m_vulkan_rhi->m_swapchain_extent.width;
			framebuffer_create_info.height			= m_vulkan_rhi->m_swapchain_extent.height;
			framebuffer_create_info.layers			= 1;

			if (VK_SUCCESS != vkCreateFramebuffer(m_vulkan_rhi->m_device, &framebuffer_create_info, nullptr, &m_framebuffers[i])) {
				MAGE_THROW(failed to create forward renderpass framebuffer)
			}
		}
	}

	void ForwardRenderPass::setupSubPasses() {
		m_p_subpasses.resize(1);
		for (int i{ 0 }; i < m_p_subpasses.size(); ++i) {
			m_p_subpasses[i] = std::make_shared<ForwardRenderSubpass>();
			ForwardRenderSubpassCreateInfo subpass_create_info{};
			subpass_create_info.m_render_pass = this;
			subpass_create_info.m_vulkan_rhi = m_vulkan_rhi;
			subpass_create_info.m_layouts_indices = { 0, 1 };
			m_p_subpasses[i]->initialize(&subpass_create_info);
		}
	}

	//subpass
	void ForwardRenderSubpass::initialize(SubpassCreateInfo* create_info) {
		Subpass::initialize(create_info);

		ForwardRenderSubpassCreateInfo* this_create_info = reinterpret_cast<ForwardRenderSubpassCreateInfo*>(create_info);
		//setupDescriptorSetLayouts(this_create_info->m_layouts_indices);
		//setupDescriptorSets(this_create_info->m_set_indices);
		setupPipeline(this_create_info->m_layouts_indices);
	}

	void ForwardRenderSubpass::setupPipeline(const std::vector<int>& indices) {
		VkGraphicsPipelineCreateInfo create_info = VulkanInfo::aboutVkGraphicsPipelineCreateInfo();
		
		//TODO:shader stages
		std::array<VkPipelineShaderStageCreateInfo, 2> stages;
		
		VkShaderModule vertex_module = nullptr;
		VkShaderModule frag_module = nullptr;

		stages[0]			= VulkanInfo::aboutVkPipelineShaderStageCreateInfo();
		stages[0].stage		= VK_SHADER_STAGE_VERTEX_BIT;
		stages[0].module	= vertex_module;
		stages[0].pName		= "main";
		stages[1]			= VulkanInfo::aboutVkPipelineShaderStageCreateInfo();
		stages[1].stage		= VK_SHADER_STAGE_FRAGMENT_BIT;
		stages[1].module	= frag_module;
		stages[1].pName		= "main";
		
		create_info.stageCount = 2;
		create_info.pStages = stages.data();

		//vertex input
		VkPipelineVertexInputStateCreateInfo vertex_input_info = VulkanInfo::aboutVkPipelineVertexInputStateCreateInfo();
		
		auto vertex_binding_descs = Vertex::getBindingDescriptions();
		auto vertex_attribute_descs = Vertex::getAttributeDescriptions();

		vertex_input_info.vertexBindingDescriptionCount		= static_cast<uint32_t>(vertex_binding_descs.size());
		vertex_input_info.pVertexBindingDescriptions		= vertex_binding_descs.data();
		vertex_input_info.vertexAttributeDescriptionCount	= static_cast<uint32_t>(vertex_attribute_descs.size());
		vertex_input_info.pVertexAttributeDescriptions		= vertex_attribute_descs.data();
		
		create_info.pVertexInputState = &vertex_input_info;

		//input assembly
		VkPipelineInputAssemblyStateCreateInfo input_assembly_info = VulkanInfo::aboutVkPipelineInputAssemblyStateCreateInfo();
		{
			input_assembly_info.topology				= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			input_assembly_info.primitiveRestartEnable	= VK_FALSE;
		}
		create_info.pInputAssemblyState = &input_assembly_info;

		//tessellation
		create_info.pTessellationState = nullptr;

		//viewport state
		VkPipelineViewportStateCreateInfo viewport_info = VulkanInfo::aboutVkPipelineViewportStateCreateInfo();

		VkViewport viewport = VulkanInfo::aboutVkViewport();
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = m_vulkan_rhi->m_swapchain_extent.width;
		viewport.height = m_vulkan_rhi->m_swapchain_extent.height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		VkRect2D scissor = VulkanInfo::aboutVkRect2D();
		scissor.offset = { 0,0 };
		scissor.extent = m_vulkan_rhi->m_swapchain_extent;

		viewport_info.viewportCount = 1;
		viewport_info.scissorCount = 1;
		viewport_info.pViewports = &viewport;
		viewport_info.pScissors = &scissor;

		create_info.pViewportState = &viewport_info;

		//rasterization
		VkPipelineRasterizationStateCreateInfo rasterization_info = VulkanInfo::aboutVkPipelineRasterizationStateCreateInfo();
		rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_info.frontFace = VK_FRONT_FACE_CLOCKWISE;

		create_info.pRasterizationState = &rasterization_info;
	}
}