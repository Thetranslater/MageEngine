#include"engine_core/render_engine/render_passes/DevRenderPass.h"

#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanInfo.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"

#include"engine_core/render_engine/render_mesh.h"

namespace Mage {

	//renderpass必须在pipeline，framebuffers之前创建，资源必须在创建subpass之前准备好。
	void DevRenderPass::initialize(const RenderPassCreateInfo* createInfo) {
		RenderPass::initialize(createInfo);

		setupDescriptorLayouts();
		setupDescriptorSets();
		setupRenderPass();
		setupSubpasses();
		setupFramebuffers(); // after create renderpass
	}

	void DevRenderPass::setupDescriptorLayouts() {}
	void DevRenderPass::setupDescriptorSets() {}
	void DevRenderPass::setupFramebuffers() {
		m_framebuffers.resize(m_vulkan_rhi->m_swapchain_size);
		for (int i = 0; i < m_framebuffers.size(); ++i) {

			{
				m_attachments.m_image_views[0] = m_vulkan_rhi->m_swapchain_image_views[i];
			}

			std::array<VkImageView, 1> imageViews = { m_attachments.m_image_views[0]};

			VkFramebufferCreateInfo createInfo = VulkanInfo::aboutVkFramebufferCreateInfo();
			createInfo.renderPass = m_render_pass;
			createInfo.attachmentCount = static_cast<uint32_t>(m_attachments.m_image_views.size());
			createInfo.pAttachments = imageViews.data();
			createInfo.width = m_vulkan_rhi->m_swapchain_extent.width;
			createInfo.height = m_vulkan_rhi->m_swapchain_extent.height;
			createInfo.layers = 1;

			if (VK_SUCCESS != vkCreateFramebuffer(m_vulkan_rhi->m_device, &createInfo, nullptr, &m_framebuffers[i])) {
				MAGE_THROW(failed to create framebuffers)
			}
		}
	}
	void DevRenderPass::setupRenderPass() {
		VkRenderPassCreateInfo createInfo = VulkanInfo::aboutVkRenderPassCreateInfo();

		m_attachments.m_attachment_descriptions.resize(1);
		m_attachments.m_image_views.resize(1);
		m_attachments.m_attachment_descriptions[0] = VulkanInfo::aboutVkAttachmentDescription();

		m_attachments.m_attachment_descriptions[0].format = m_vulkan_rhi->m_swapchain_surface_format.format;
		m_attachments.m_attachment_descriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		m_attachments.m_attachment_descriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		m_attachments.m_attachment_descriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		m_attachments.m_attachment_descriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		m_attachments.m_attachment_descriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_attachments.m_attachment_descriptions[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		createInfo.attachmentCount = static_cast<uint32_t>(m_attachments.m_attachment_descriptions.size());
		createInfo.pAttachments = m_attachments.m_attachment_descriptions.data();
		createInfo.subpassCount = m_subpass_count;

		std::vector<VkSubpassDescription> subpass_descriptions;
		VkAttachmentReference color_attachment_ref = VulkanInfo::aboutVkAttachmentReference();
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpass_descriptions.resize(m_subpass_count);
		subpass_descriptions[0] = VulkanInfo::aboutVkSubpassDescription();

		subpass_descriptions[0].inputAttachmentCount = 0;
		subpass_descriptions[0].pInputAttachments = nullptr;
		subpass_descriptions[0].colorAttachmentCount = 1;
		subpass_descriptions[0].pColorAttachments = &color_attachment_ref;

		createInfo.pSubpasses = subpass_descriptions.data();

		if (VK_SUCCESS != vkCreateRenderPass(m_vulkan_rhi->m_device, &createInfo, nullptr, &m_render_pass)) {
			MAGE_THROW(failed to create renderpass)
		}
	}
	void DevRenderPass::setupSubpasses() {
		m_p_subpasses.resize(1);
		m_p_subpasses[0] = std::make_shared<DevRenderSubpass>();
		SubpassCreateInfo createInfo;
		createInfo.m_vulkan_rhi = m_vulkan_rhi;
		createInfo.m_render_pass = this;
		for (auto& subpass : m_p_subpasses) {
			subpass->initialize(&createInfo);
		}
	}

	void DevRenderPass::draw() {
		VkRenderPassBeginInfo pass_begin_info = VulkanInfo::aboutVkRenderPassBeginInfo();
		pass_begin_info.renderPass = m_render_pass;
		pass_begin_info.framebuffer = m_framebuffers[m_vulkan_rhi->m_swapchain_image_index];
		pass_begin_info.clearValueCount = 1;
		pass_begin_info.renderArea.offset = { 0,0 };
		pass_begin_info.renderArea.extent = m_vulkan_rhi->m_swapchain_extent;

		VkClearValue clear_values = VulkanInfo::aboutVkClearValue();
		clear_values.color = { 0.f,0.f,0.f,0.f };

		pass_begin_info.pClearValues = &clear_values;

		vkCmdBeginRenderPass(m_vulkan_rhi->m_command_buffer, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		
		VkDeviceSize vertex_buffer_offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_vulkan_rhi->m_command_buffer, 0, 1, &m_resources->m_vertex_buffer, vertex_buffer_offsets);

		vkCmdBindIndexBuffer(m_vulkan_rhi->m_command_buffer, m_resources->m_index_buffer, 0, VK_INDEX_TYPE_UINT16);

		for (auto& subpass : m_p_subpasses) {
			subpass->draw();
		};

		vkCmdEndRenderPass(m_vulkan_rhi->m_command_buffer);
	}

//subpass creation
	void DevRenderSubpass::initialize(const SubpassCreateInfo* createInfo) {
		Subpass::initialize(createInfo);

		setupDescriptorSetLayouts({});
		setupDescriptorSets({});
		setupPipeline();
	}	

	void DevRenderSubpass::draw() {
		vkCmdBindPipeline(m_vulkan_rhi->m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

		vkCmdDrawIndexed(m_vulkan_rhi->m_command_buffer, 6, 1, 0, 0, 0);
	}

	void DevRenderSubpass::setupDescriptorSetLayouts(const std::vector<int>& indices) {}
	void DevRenderSubpass::setupDescriptorSets(const std::vector<int>& indices) {}
	void DevRenderSubpass::setupPipeline() {
		VkGraphicsPipelineCreateInfo createInfo = VulkanInfo::aboutVkGraphicsPipelineCreateInfo();

		//shader stage
		VkShaderModule vertexModule = VulkanHelper::shaderModuleCreationHelper(m_vulkan_rhi->m_device, "E:/ProgramingFile/C++/Mage/Engine/shaders/devvert.spv");
		VkShaderModule fragModule	= VulkanHelper::shaderModuleCreationHelper(m_vulkan_rhi->m_device, "E:/ProgramingFile/C++/Mage/Engine/shaders/devfrag.spv");

		std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages;
		{
			shader_stages[0]		= VulkanInfo::aboutVkPipelineShaderStageCreateInfo();
			shader_stages[0].stage	= VK_SHADER_STAGE_VERTEX_BIT;
			shader_stages[0].module = vertexModule;
			shader_stages[0].pName	= "main";
			
			shader_stages[1]		= VulkanInfo::aboutVkPipelineShaderStageCreateInfo();
			shader_stages[1].stage	= VK_SHADER_STAGE_FRAGMENT_BIT;
			shader_stages[1].module = fragModule;
			shader_stages[1].pName	= "main";
		}

		createInfo.stageCount	= 2;
		createInfo.pStages		= shader_stages.data();

		//vertex input
		VkPipelineVertexInputStateCreateInfo vertex_input = VulkanInfo::aboutVkPipelineVertexInputStateCreateInfo();

		auto vertex_input_bindings		= Vertex::getBindingDescriptions();
		auto vertex_input_attributes	= Vertex::getAttributeDescriptions();

		vertex_input.vertexBindingDescriptionCount		= static_cast<uint32_t>(vertex_input_bindings.size());
		vertex_input.pVertexBindingDescriptions			= vertex_input_bindings.data();
		vertex_input.vertexAttributeDescriptionCount	= static_cast<uint32_t>(vertex_input_attributes.size());
		vertex_input.pVertexAttributeDescriptions		= vertex_input_attributes.data();

		createInfo.pVertexInputState = &vertex_input;

		//input assembly
		VkPipelineInputAssemblyStateCreateInfo input_assembly = VulkanInfo::aboutVkPipelineInputAssemblyStateCreateInfo();
		input_assembly.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable	= VK_FALSE;

		createInfo.pInputAssemblyState = &input_assembly;

		//viewport state
		VkViewport view_port = VulkanInfo::aboutVkViewport();
		view_port.x = 0.f;
		view_port.y = 0.f;
		view_port.width = m_vulkan_rhi->m_swapchain_extent.width;
		view_port.height = m_vulkan_rhi->m_swapchain_extent.height;
		view_port.minDepth = 0.f;
		view_port.maxDepth = 1.f;
		
		VkRect2D scissor = VulkanInfo::aboutVkRect2D();
		scissor.offset = { 0,0 };
		scissor.extent = m_vulkan_rhi->m_swapchain_extent;

		VkPipelineViewportStateCreateInfo view_port_state = VulkanInfo::aboutVkPipelineViewportStateCreateInfo();
		view_port_state.viewportCount = 1;
		view_port_state.pViewports = &view_port;
		view_port_state.scissorCount = 1;
		view_port_state.pScissors = &scissor;

		createInfo.pViewportState = &view_port_state;

		//rasterization
		VkPipelineRasterizationStateCreateInfo rasterization = VulkanInfo::aboutVkPipelineRasterizationStateCreateInfo();
		rasterization.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;

		createInfo.pRasterizationState = &rasterization;

		//color blend
		VkPipelineColorBlendStateCreateInfo color_blend = VulkanInfo::aboutVkPipelineColorBlendStateCreateInfo();
		color_blend.logicOpEnable = VK_FALSE;
		color_blend.attachmentCount = 1;

		std::array<VkPipelineColorBlendAttachmentState, 1> attachment_blend;
		{
			attachment_blend[0] = VulkanInfo::aboutVkPipelineColorBlendAttachmentState();
			attachment_blend[0].blendEnable = VK_FALSE;
		}

		color_blend.pAttachments = attachment_blend.data();
		
		createInfo.pColorBlendState = &color_blend;

		//layout
		VkPipelineLayout layout;

		VkPipelineLayoutCreateInfo layout_create_info = VulkanInfo::aboutVkPipelineLayoutCreateInfo();
		layout_create_info.setLayoutCount = 0;
		layout_create_info.pushConstantRangeCount = 0;

		if (VK_SUCCESS != vkCreatePipelineLayout(m_vulkan_rhi->m_device, &layout_create_info, nullptr, &layout)) {
			MAGE_THROW(failed to create pipeline layout)
		}
		createInfo.layout = layout;

		//multisample
		VkPipelineMultisampleStateCreateInfo multisample_create_info = VulkanInfo::aboutVkPipelineMultisampleStateCreateInfo();
		multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		createInfo.pMultisampleState = &multisample_create_info;

		//others
		createInfo.pDepthStencilState = nullptr;
		createInfo.pDynamicState = nullptr;
		createInfo.renderPass = p_m_render_pass->getVkRenderPass();
		createInfo.subpass = 0;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (VK_SUCCESS != vkCreateGraphicsPipelines(m_vulkan_rhi->m_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_pipeline)) {
			MAGE_THROW(failed to create pipeline)
		}	
	}


}