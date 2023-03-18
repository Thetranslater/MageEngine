#include"engine_core/render_system/renderer/vulkanInfo.h"
#include"engine_core/render_system/renderer/vulkanRHI.h"
#include"engine_core/render_system/renderer/vulkanHelper.h"
#include"engine_core/render_system/render_mesh.h"
#include"engine_core/render_system/render_resource.h"
#include"engine_core/render_system/render_system.h"
#include"engine_core/render_system/render_passes/DirectionalShadowPass.h"

namespace Mage {
	enum {
		directional_shadow_color = 0,
		directional_shadow_depth
	};

	void DirectionalShadowPass::initialize(const RenderPassCreateInfo* info) {
		RenderPass::initialize(info);

		setupDescriptorLayouts();
		setupDescriptorSets();
		setupAttachments();
		setupRenderPass();
		setupSubPasses();
		setupFramebuffers();
	}

	void DirectionalShadowPass::setupDescriptorLayouts() {
		m_descriptor_sets.layout_infos.resize(1);

		VkDescriptorSetLayoutCreateInfo global_mesh_set = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();

		VkDescriptorSetLayoutBinding global_perframe_layout_binding_Vertex = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
		global_perframe_layout_binding_Vertex.binding = 0;
		global_perframe_layout_binding_Vertex.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		global_perframe_layout_binding_Vertex.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding global_perdrawcall_layout_binding_Vertex = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
		global_perdrawcall_layout_binding_Vertex.binding = 1;
		global_perdrawcall_layout_binding_Vertex.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		global_perdrawcall_layout_binding_Vertex.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> global_setlayout = {
			global_perframe_layout_binding_Vertex,
			global_perdrawcall_layout_binding_Vertex };

		global_mesh_set.bindingCount = 2;
		global_mesh_set.pBindings = global_setlayout.data();
		if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->getDevice(), &global_mesh_set, nullptr, &m_descriptor_sets.layout_infos[0])) {
			MAGE_THROW(failed to create forward renderpass descriptor set layout)
		}
	}

	void DirectionalShadowPass::setupDescriptorSets() {
		m_descriptor_sets.sets.resize(m_vulkan_rhi->getSwapchainSize());
		{
			std::vector<VkDescriptorSetLayout> layouts(m_vulkan_rhi->getSwapchainSize(), m_descriptor_sets.layout_infos[0]);
			VkDescriptorSetAllocateInfo global_descriptors_allocate_info_Vertex = VulkanInfo::aboutVkDescriptorSetAllocateInfo();
			global_descriptors_allocate_info_Vertex.descriptorPool = m_vulkan_rhi->getDescriptorPool();
			global_descriptors_allocate_info_Vertex.descriptorSetCount = m_vulkan_rhi->getSwapchainSize();
			global_descriptors_allocate_info_Vertex.pSetLayouts = layouts.data();

			if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_rhi->getDevice(), &global_descriptors_allocate_info_Vertex, m_descriptor_sets.sets.data())) {
				MAGE_THROW(failde to allocate descriptor sets of forward renderpass)
			}

			for (int i{ 0 }; i < m_vulkan_rhi->getSwapchainSize(); ++i) {
				VkDescriptorBufferInfo buffer_info[2] = { VulkanInfo::aboutVkDescriptorBufferInfo() };
				VkWriteDescriptorSet writes[2] = { VulkanInfo::aboutVkWriteDescriptorSet() };
				for (int j{ 0 }; j < 2; ++j) {
					buffer_info[j] = VulkanInfo::aboutVkDescriptorBufferInfo();
					buffer_info[j].buffer = m_render_system->getRenderResource()->m_global_updated_buffer.m_buffers[i];
					buffer_info[j].offset = 0;

					writes[j] = VulkanInfo::aboutVkWriteDescriptorSet();
					writes[j].dstArrayElement = 0;
					writes[j].dstSet = m_descriptor_sets.sets[i];
					writes[j].dstBinding = j;
					writes[j].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
					writes[j].descriptorCount = 1;
				}
				buffer_info[0].range = sizeof(GlobalBufferPerFrameData);
				buffer_info[1].range = sizeof(GlobalBufferPerDrawcallData);

				writes[0].pBufferInfo = &buffer_info[0];
				writes[1].pBufferInfo = &buffer_info[1];

				vkUpdateDescriptorSets(m_vulkan_rhi->getDevice(), 2, writes, 0, nullptr);
			}
		}
	}

	void DirectionalShadowPass::setupAttachments() {
		m_attachments.resize(2);
		{
			//directional_shadow
			m_attachments.m_attachment_descriptions[directional_shadow_color] = VulkanInfo::aboutVkAttachmentDescription();
			m_attachments.m_attachment_descriptions[directional_shadow_color].format = VK_FORMAT_R32_SFLOAT;
			m_attachments.m_attachment_descriptions[directional_shadow_color].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			m_attachments.m_attachment_descriptions[directional_shadow_color].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			m_attachments.m_attachment_descriptions[directional_shadow_color].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[directional_shadow_color].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[directional_shadow_color].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			m_attachments.m_attachment_descriptions[directional_shadow_color].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			m_attachments.m_attachment_descriptions[directional_shadow_depth] = VulkanInfo::aboutVkAttachmentDescription();
			m_attachments.m_attachment_descriptions[directional_shadow_depth].format = m_vulkan_rhi->getDepthImageFormat();
			m_attachments.m_attachment_descriptions[directional_shadow_depth].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			m_attachments.m_attachment_descriptions[directional_shadow_depth].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[directional_shadow_depth].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[directional_shadow_depth].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_attachments.m_attachment_descriptions[directional_shadow_depth].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			m_attachments.m_attachment_descriptions[directional_shadow_depth].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		//create
		VulkanHelper::imageCreationHelper(
			m_vulkan_rhi.get(),
			DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE,
			DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE,
			VK_FORMAT_R32_SFLOAT, 1, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_attachments.m_images[directional_shadow_color], m_attachments.m_image_memories[directional_shadow_color]);
		VulkanHelper::imageViewCreationHelper(
			m_vulkan_rhi.get(), 
			m_attachments.m_images[directional_shadow_color], 
			VK_FORMAT_R32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT, 1, 
			m_attachments.m_image_views[directional_shadow_color]);

		VulkanHelper::imageCreationHelper(
			m_vulkan_rhi.get(),
			DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE,
			DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE,
			m_vulkan_rhi->getDepthImageFormat(), 1, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_attachments.m_images[directional_shadow_depth], m_attachments.m_image_memories[directional_shadow_depth]);
		VulkanHelper::imageViewCreationHelper(
			m_vulkan_rhi.get(),
			m_attachments.m_images[directional_shadow_depth],
			m_vulkan_rhi->getDepthImageFormat(), VK_IMAGE_ASPECT_DEPTH_BIT, 1,
			m_attachments.m_image_views[directional_shadow_depth]);
	}

	void DirectionalShadowPass::setupRenderPass() {
		VkRenderPassCreateInfo info = VulkanInfo::aboutVkRenderPassCreateInfo();
		info.attachmentCount = 2;
		info.pAttachments = m_attachments.m_attachment_descriptions.data();

		VkSubpassDescription shadow_pass_description = VulkanInfo::aboutVkSubpassDescription();
		shadow_pass_description.colorAttachmentCount = 1;
		shadow_pass_description.inputAttachmentCount = 0;
		shadow_pass_description.preserveAttachmentCount = 0;
		VkAttachmentReference color_ref = VulkanInfo::aboutVkAttachmentReference();
		color_ref.attachment = directional_shadow_color;
		color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkAttachmentReference depth_ref = VulkanInfo::aboutVkAttachmentReference();
		depth_ref.attachment = directional_shadow_depth;
		depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		shadow_pass_description.pColorAttachments = &color_ref;
		shadow_pass_description.pDepthStencilAttachment = &depth_ref;
		shadow_pass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		
		VkSubpassDependency shadow_pass_dependency = VulkanInfo::aboutVkSubpassDependency();
		shadow_pass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		shadow_pass_dependency.dstSubpass = 0;
		shadow_pass_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		shadow_pass_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		shadow_pass_dependency.srcAccessMask = 0;
		shadow_pass_dependency.dstAccessMask = 0;

		info.dependencyCount = 1;
		info.subpassCount = 1;
		info.pDependencies = &shadow_pass_dependency;
		info.pSubpasses = &shadow_pass_description;

		ASSERT_RESULT(vkCreateRenderPass(m_vulkan_rhi->getDevice(), &info, VK_NULL_HANDLE, &m_render_pass));
	}

	void DirectionalShadowPass::setupSubPasses() {
		m_p_subpasses.resize(m_subpass_count);
		m_p_subpasses[0] = std::make_shared<DirectionalShadowSubpass>();
		DirectionalShadowSubpassCreateInfo subpass_info{};
		subpass_info.info_vulkan_rhi = m_vulkan_rhi;
		subpass_info.info_render_pass = this;
		subpass_info.info_layout_indices = { 0 };
		m_p_subpasses[0]->initialize(&subpass_info);
	}

	void DirectionalShadowPass::setupFramebuffers() {
		m_framebuffers.resize(1);
		VkFramebufferCreateInfo info = VulkanInfo::aboutVkFramebufferCreateInfo();
		info.renderPass = m_render_pass;
		info.attachmentCount = 2;
		info.pAttachments = m_attachments.m_image_views.data();
		info.width = DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE;
		info.height = DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE;
		info.layers = 1;

		ASSERT_RESULT(vkCreateFramebuffer(m_vulkan_rhi->getDevice(), &info, VK_NULL_HANDLE, &m_framebuffers[0]));
	}

	void DirectionalShadowPass::draw() {
		VkRenderPassBeginInfo begin_info = VulkanInfo::aboutVkRenderPassBeginInfo();
		begin_info.renderPass = m_render_pass;
		begin_info.framebuffer = m_framebuffers[0];
		begin_info.renderArea.offset = { 0,0 };
		begin_info.renderArea.extent = { DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE, DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE };
		begin_info.clearValueCount = 2;
		VkClearValue clears[2];
		clears[0].color = { 0.f,0.f,0.f,1.f };
		clears[1].depthStencil = { 1.f,0 };
		begin_info.pClearValues = clears;

		vkCmdBeginRenderPass(m_vulkan_rhi->getCurrentCommandBuffer(), &begin_info, VK_SUBPASS_CONTENTS_INLINE);

		m_p_subpasses[0]->draw();
	}

	void DirectionalShadowSubpass::initialize(SubpassCreateInfo* info) {
		Subpass::initialize(info);
		DirectionalShadowSubpassCreateInfo* shadow_info = static_cast<DirectionalShadowSubpassCreateInfo*>(info);

		setupPipeline(shadow_info->info_layout_indices);
	}

	void DirectionalShadowSubpass::setupPipeline(const std::vector<int>& indices) {
		VkGraphicsPipelineCreateInfo pipeline_info = VulkanInfo::aboutVkGraphicsPipelineCreateInfo();

		//TODO:shader stage
		VkPipelineShaderStageCreateInfo shader_info[2];

		pipeline_info.stageCount = 2;
		pipeline_info.pStages = shader_info;

		//vertex input
		VkPipelineVertexInputStateCreateInfo vertex_input_info = VulkanInfo::aboutVkPipelineVertexInputStateCreateInfo();
		auto attribute_desc = Vertex::getAttributeDescriptions();
		auto binding_desc = Vertex::getBindingDescriptions();
		vertex_input_info.pVertexAttributeDescriptions = attribute_desc.data();
		vertex_input_info.pVertexBindingDescriptions = binding_desc.data();
		vertex_input_info.vertexAttributeDescriptionCount = attribute_desc.size();
		vertex_input_info.vertexBindingDescriptionCount = binding_desc.size();
		pipeline_info.pVertexInputState = &vertex_input_info;

		//input assembly
		VkPipelineInputAssemblyStateCreateInfo input_assembly_info = VulkanInfo::aboutVkPipelineInputAssemblyStateCreateInfo();
		input_assembly_info.primitiveRestartEnable = VK_FALSE;
		input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipeline_info.pInputAssemblyState = &input_assembly_info;

		//viewport state
		VkPipelineViewportStateCreateInfo viewport_info = VulkanInfo::aboutVkPipelineViewportStateCreateInfo();
		viewport_info.scissorCount = 1;
		viewport_info.viewportCount = 1;
		VkViewport viewport = VulkanInfo::aboutVkViewport();
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE;
		viewport.height = DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		VkRect2D scissor = VulkanInfo::aboutVkRect2D();
		scissor.offset = { 0,0 };
		scissor.extent = { DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE, DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE };
		viewport_info.pViewports = &viewport;
		viewport_info.pScissors = &scissor;

		//rasterization
		VkPipelineRasterizationStateCreateInfo rasterization_info = VulkanInfo::aboutVkPipelineRasterizationStateCreateInfo();
		rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		//multisample
		VkPipelineMultisampleStateCreateInfo multisample_info = VulkanInfo::aboutVkPipelineMultisampleStateCreateInfo();
		pipeline_info.pMultisampleState = &multisample_info;

		//depth stencil
		VkPipelineDepthStencilStateCreateInfo depth_stencil_info = VulkanInfo::aboutVkPipelineDepthStencilStateCreateInfo();
		depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
		depth_stencil_info.depthTestEnable = VK_TRUE;
		depth_stencil_info.depthWriteEnable = VK_TRUE;
		depth_stencil_info.minDepthBounds = 0.f;
		depth_stencil_info.maxDepthBounds = 1.f;
		pipeline_info.pDepthStencilState = &depth_stencil_info;

		//blend
		VkPipelineColorBlendStateCreateInfo blend_info = VulkanInfo::aboutVkPipelineColorBlendStateCreateInfo();
		blend_info.attachmentCount = 1;
		blend_info.logicOpEnable = VK_FALSE;
		VkPipelineColorBlendAttachmentState blend_state = VulkanInfo::aboutVkPipelineColorBlendAttachmentState();
		blend_state.blendEnable = VK_FALSE;
		blend_info.pAttachments = &blend_state;
		pipeline_info.pColorBlendState = &blend_info;

		//dynamic state
		VkPipelineDynamicStateCreateInfo dynamic_info = VulkanInfo::aboutVkPipelineDynamicStateCreateInfo();
		dynamic_info.dynamicStateCount = 2;
		VkDynamicState dynamics[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, /*VK_DYNAMIC_STATE_VERTEX_INPUT_EXT*/ };
		dynamic_info.pDynamicStates = dynamics;

		//layout
		VkPipelineLayoutCreateInfo pipeline_layout_info = VulkanInfo::aboutVkPipelineLayoutCreateInfo();
		pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(indices.size());
		std::vector<VkDescriptorSetLayout> layouts(indices.size());
		for (int i{ 0 }; i < indices.size(); ++i) layouts[i] = p_m_render_pass->m_descriptor_sets.layout_infos[indices[i]];
		pipeline_layout_info.pSetLayouts = layouts.data();
		
		if (VK_SUCCESS != vkCreatePipelineLayout(m_vulkan_rhi->getDevice(), &pipeline_layout_info, VK_NULL_HANDLE, &m_pipeline_layout)) {
			MAGE_THROW(cant create pipeline layout of the shadow pass)
		}
		pipeline_info.layout = m_pipeline_layout;

		pipeline_info.renderPass = p_m_render_pass->m_render_pass;
		pipeline_info.subpass = 0;

		if (VK_SUCCESS != vkCreateGraphicsPipelines(m_vulkan_rhi->getDevice(), VK_NULL_HANDLE, 1, &pipeline_info, VK_NULL_HANDLE, &m_pipeline)) {
			MAGE_THROW(failed to create shadow pass)
		}
	}

	void DirectionalShadowSubpass::draw() {

	}
}