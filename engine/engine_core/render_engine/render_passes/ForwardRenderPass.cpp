#include"engine_core/render_engine/render_passes/ForwardRenderPass.h"

#include"engine_core/render_engine/renderer/vulkanInfo.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"

#include"engine_core/render_engine/render_macro.h"
#include"engine_core/render_engine/render_mesh.h"
#include"engine_core/render_engine/render_resource.h"
#include"engine_core/render_engine/render_scene.h"

#include"core/math/math.h"
#include"core/math/matrix4x4.h"

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
		m_descriptor_sets.layout_infos.resize(4);
		//set 1-3:global resources, lights, camera, sky
		{
			//TODO
			VkDescriptorSetLayoutBinding global_perframe_layout_binding_Vertex = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			global_perframe_layout_binding_Vertex.binding				= 0;
			global_perframe_layout_binding_Vertex.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perframe_layout_binding_Vertex.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutBinding global_perdrawcall_layout_binding_Vertex = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			global_perdrawcall_layout_binding_Vertex.binding			= 1;
			global_perdrawcall_layout_binding_Vertex.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perdrawcall_layout_binding_Vertex.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutBinding global_perdrawcall_layout_binding_Frag = VulkanInfo::aboutVkDescriptorSetLayoutBinding();
			global_perdrawcall_layout_binding_Frag.binding = 2;
			global_perdrawcall_layout_binding_Frag.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			global_perdrawcall_layout_binding_Frag.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			std::array<VkDescriptorSetLayoutBinding, 3> global_setlayout = {
				global_perframe_layout_binding_Vertex,
				global_perdrawcall_layout_binding_Vertex,
				global_perdrawcall_layout_binding_Frag };

			VkDescriptorSetLayoutCreateInfo set_1 = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
			set_1.bindingCount = 3;
			set_1.pBindings = global_setlayout.data();
			if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device, &set_1, nullptr, &m_descriptor_sets.layout_infos[0])) {
				MAGE_THROW(failed to create forward renderpass descriptor set layout)
			}

			m_descriptor_sets.layout_infos[1] = m_descriptor_sets.layout_infos[2] = m_descriptor_sets.layout_infos[0];
		}

		//set 4: custom textures
		{
			VkDescriptorSetLayoutCreateInfo set_3 = VulkanInfo::aboutVkDescriptorSetLayoutCreateInfo();
			set_3.bindingCount = 3;
			//bindings
			std::array<VkDescriptorSetLayoutBinding, 3> set_3_bindings;
			{
				//albedo
				set_3_bindings[0]						= VulkanInfo::aboutVkDescriptorSetLayoutBinding();
				set_3_bindings[0].binding				= 0;
				set_3_bindings[0].descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				set_3_bindings[0].descriptorCount		= 1;
				set_3_bindings[0].stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT;
				set_3_bindings[0].pImmutableSamplers	= nullptr;
				//normal
				set_3_bindings[1]			= set_3_bindings[0];
				set_3_bindings[1].binding	= 1;
				//metallic-roughness
				set_3_bindings[2]			= set_3_bindings[0];
				set_3_bindings[2].binding	= 2;
			}
			set_3.pBindings = set_3_bindings.data();
			if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device, &set_3, nullptr, &m_descriptor_sets.layout_infos[3])) {
				MAGE_THROW(failed to create forward renderpass descriptor set layout)
			}
		}
	}

	void ForwardRenderPass::setupDescriptorSets() {
		m_descriptor_sets.sets.resize(4);
		//set_1-3 create
		{
			VkDescriptorSetAllocateInfo global_descriptors_allocate_info_Vertex	= VulkanInfo::aboutVkDescriptorSetAllocateInfo();
			global_descriptors_allocate_info_Vertex.descriptorPool		= m_vulkan_rhi->m_descriptor_pool;
			global_descriptors_allocate_info_Vertex.descriptorSetCount	= 3;
			global_descriptors_allocate_info_Vertex.pSetLayouts = m_descriptor_sets.layout_infos.data();

			if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_rhi->m_device,&global_descriptors_allocate_info_Vertex,m_descriptor_sets.sets.data())) {
				MAGE_THROW(failde to allocate descriptor sets of forward renderpass)
			}

			for (int i{ 0 }; i < 3; ++i) {
				VkDescriptorBufferInfo buffer_info[3] = { VulkanInfo::aboutVkDescriptorBufferInfo()};
				VkWriteDescriptorSet writes[3] = { VulkanInfo::aboutVkWriteDescriptorSet() };
				for (int j{ 0 }; j < 3; ++j) {
					buffer_info[j].buffer = m_render_resource->m_global_updated_buffer.m_buffers[i];
					buffer_info[j].offset = 0;

					writes[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writes[j].dstArrayElement = 0;
					writes[j].dstSet = m_descriptor_sets.sets[i];
					writes[j].dstBinding = j;
					writes[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					writes[j].descriptorCount = 1;
				}
				buffer_info[0].range = sizeof(GlobalBufferPerFrameData);
				buffer_info[1].range = sizeof(GlobalBufferPerDrawcallVertexShaderData);
				buffer_info[2].range = sizeof(GlobalBufferPerDrawcallFragmentShaderData);

				writes[0].pBufferInfo = &buffer_info[0];
				writes[1].pBufferInfo = &buffer_info[1];
				writes[2].pBufferInfo = &buffer_info[2];

				vkUpdateDescriptorSets(m_vulkan_rhi->m_device, 3, writes, 0, nullptr);
			}
		}
		//set 3 create
		{
			VkDescriptorSetAllocateInfo texture_descriptors_allocate_info = VulkanInfo::aboutVkDescriptorSetAllocateInfo();
			texture_descriptors_allocate_info.descriptorPool = m_vulkan_rhi->m_descriptor_pool;
			texture_descriptors_allocate_info.descriptorSetCount = 1;
			texture_descriptors_allocate_info.pSetLayouts = &m_descriptor_sets.layout_infos[3];

			if (VK_SUCCESS != vkAllocateDescriptorSets(m_vulkan_rhi->m_device, &texture_descriptors_allocate_info, &m_descriptor_sets.sets[3])) {
				MAGE_THROW(failed to allocate forward renderpass descritor set_3)
			}
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
			directional_lighting_subpass_depend.srcStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			directional_lighting_subpass_depend.srcAccessMask	= 0;
			directional_lighting_subpass_depend.dstStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			directional_lighting_subpass_depend.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
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
			subpass_create_info.m_layouts_indices = { 0, 3 };
			m_p_subpasses[i]->initialize(&subpass_create_info);
		}
	}

	//TODO::现在我们只有一个subpass，所以直接调用subpass的draw即可,资源都在scene和resource类中拿。
	void ForwardRenderPass::draw() {
		VkRenderPassBeginInfo pass_begin_info = VulkanInfo::aboutVkRenderPassBeginInfo();
		pass_begin_info.renderPass = m_render_pass;
		pass_begin_info.framebuffer = m_framebuffers[m_vulkan_rhi->m_swapchain_image_index];
		pass_begin_info.renderArea.offset = { 0,0 };
		pass_begin_info.renderArea.extent = m_vulkan_rhi->m_swapchain_extent;
		pass_begin_info.clearValueCount = 2;
		VkClearValue clears[2] = { {{0.2f,0.3f,0.4f,1.f}},{{0.f,0.f,0.f,0.f}} };
		pass_begin_info.pClearValues = clears;
		vkCmdBeginRenderPass(m_vulkan_rhi->m_command_buffer, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		m_p_subpasses[0]->draw();
	}

	void ForwardRenderPass::rebindGlobalBuffer() {
		VkBuffer current_global_buffer_wait_for_update = m_render_resource->m_global_updated_buffer.m_buffers[m_vulkan_rhi->m_current_frame_index];
		void* map_pointer = m_render_resource->m_global_updated_buffer.m_followed_camera_updated_data_pointers[m_vulkan_rhi->m_current_frame_index];

		VkDescriptorBufferInfo descriptor_buffer_info = VulkanInfo::aboutVkDescriptorBufferInfo();
		descriptor_buffer_info.buffer = current_global_buffer_wait_for_update;
		descriptor_buffer_info.offset = 0;
		descriptor_buffer_info.range = sizeof(GlobalBufferPerFrameData);
		VkWriteDescriptorSet global_perframe_write = VulkanInfo::aboutVkWriteDescriptorSet();
		global_perframe_write.dstBinding = 0;
		global_perframe_write.dstSet = m_descriptor_sets.sets[m_vulkan_rhi->m_current_frame_index];
		global_perframe_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		global_perframe_write.pBufferInfo = &descriptor_buffer_info;

		VkDescriptorBufferInfo buffer_info = VulkanInfo::aboutVkDescriptorBufferInfo();
		buffer_info.buffer = current_global_buffer_wait_for_update;
		buffer_info.offset = 0;
		buffer_info.range = sizeof(GlobalBufferPerDrawcallVertexShaderData);
		VkWriteDescriptorSet global_perdrawcall_write = VulkanInfo::aboutVkWriteDescriptorSet();
		global_perdrawcall_write.dstSet = m_descriptor_sets.sets[m_vulkan_rhi->m_current_frame_index];
		global_perdrawcall_write.dstBinding = 1;
		global_perdrawcall_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		global_perdrawcall_write.pBufferInfo = &buffer_info;

		VkDescriptorBufferInfo frag_buffer_info = VulkanInfo::aboutVkDescriptorBufferInfo();
		frag_buffer_info.buffer = current_global_buffer_wait_for_update;
		frag_buffer_info.offset = 0;
		frag_buffer_info.range = sizeof(GlobalBufferPerDrawcallFragmentShaderData);
		VkWriteDescriptorSet global_perdrawcall_frag_write = VulkanInfo::aboutVkWriteDescriptorSet();
		global_perdrawcall_frag_write.dstSet = m_descriptor_sets.sets[m_vulkan_rhi->m_current_frame_index];
		global_perdrawcall_frag_write.dstBinding = 2;
		global_perdrawcall_frag_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		global_perdrawcall_frag_write.pBufferInfo = &frag_buffer_info;

		VkWriteDescriptorSet writes[3] = { global_perframe_write,global_perdrawcall_write,global_perdrawcall_frag_write };
		vkUpdateDescriptorSets(m_vulkan_rhi->m_device, 3, writes, 0, nullptr);
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
		
		VkShaderModule vertex_module = VulkanHelper::shaderModuleCreationHelper(m_vulkan_rhi->m_device, "E:/Mage/engine/shaders/forwardVERT.spv");
		VkShaderModule frag_module = VulkanHelper::shaderModuleCreationHelper(m_vulkan_rhi->m_device, "E:/Mage/engine/shaders/forwardFRAG.spv");

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

		vertex_input_info.vertexBindingDescriptionCount		= static_cast<uint32_t>(vertex_binding_descs.size()) - 2;
		vertex_input_info.pVertexBindingDescriptions		= vertex_binding_descs.data();
		vertex_input_info.vertexAttributeDescriptionCount	= static_cast<uint32_t>(vertex_attribute_descs.size()) - 2;
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
		viewport.x			= 0.f;
		viewport.y			= 0.f;
		viewport.width		= m_vulkan_rhi->m_swapchain_extent.width;
		viewport.height		= m_vulkan_rhi->m_swapchain_extent.height;
		viewport.minDepth	= 0.f;
		viewport.maxDepth	= 1.f;

		VkRect2D scissor = VulkanInfo::aboutVkRect2D();
		scissor.offset = { 0,0 };
		scissor.extent = m_vulkan_rhi->m_swapchain_extent;

		viewport_info.viewportCount = 1;
		viewport_info.scissorCount	= 1;
		viewport_info.pViewports	= &viewport;
		viewport_info.pScissors		= &scissor;

		create_info.pViewportState = &viewport_info;

		//rasterization
		VkPipelineRasterizationStateCreateInfo rasterization_info = VulkanInfo::aboutVkPipelineRasterizationStateCreateInfo();
		rasterization_info.polygonMode	= VK_POLYGON_MODE_FILL;
		rasterization_info.cullMode		= VK_CULL_MODE_NONE;
		rasterization_info.frontFace	= VK_FRONT_FACE_CLOCKWISE;

		create_info.pRasterizationState = &rasterization_info;

		//multisampler
		VkPipelineMultisampleStateCreateInfo multisample_info = VulkanInfo::aboutVkPipelineMultisampleStateCreateInfo();
		multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		create_info.pMultisampleState = &multisample_info;

		//depth_stencil
		VkPipelineDepthStencilStateCreateInfo depth_stencil_info = VulkanInfo::aboutVkPipelineDepthStencilStateCreateInfo();
		depth_stencil_info.depthTestEnable	= VK_TRUE;
		depth_stencil_info.depthWriteEnable	= VK_TRUE;
		depth_stencil_info.depthCompareOp	= VK_COMPARE_OP_LESS;
		
		create_info.pDepthStencilState = &depth_stencil_info;

		//color blend
		VkPipelineColorBlendAttachmentState blend_in_color_attachments = VulkanInfo::aboutVkPipelineColorBlendAttachmentState();
		blend_in_color_attachments.blendEnable		= VK_FALSE;
		blend_in_color_attachments.colorWriteMask	= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	
		VkPipelineColorBlendStateCreateInfo color_blend_info = VulkanInfo::aboutVkPipelineColorBlendStateCreateInfo();
		color_blend_info.logicOpEnable = VK_FALSE;
		color_blend_info.attachmentCount = 1;
		color_blend_info.pAttachments = &blend_in_color_attachments;

		create_info.pColorBlendState = &color_blend_info;

		//dynamic state
		VkPipelineDynamicStateCreateInfo dynamic_info = VulkanInfo::aboutVkPipelineDynamicStateCreateInfo();
		dynamic_info.dynamicStateCount = 2;
		VkDynamicState dynamics[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, /*VK_DYNAMIC_STATE_VERTEX_INPUT_EXT*/};
		dynamic_info.pDynamicStates = dynamics;

		create_info.pDynamicState = &dynamic_info;

		//pipelinelayout
		VkPipelineLayoutCreateInfo pipeline_layout_info = VulkanInfo::aboutVkPipelineLayoutCreateInfo();
		pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(indices.size());
		std::vector<VkDescriptorSetLayout> layouts(indices.size());
		for (int i{ 0 }; i < indices.size(); ++i) layouts[i] = p_m_render_pass->m_descriptor_sets.layout_infos[indices[i]];
		pipeline_layout_info.pSetLayouts = layouts.data();

		if (VK_SUCCESS != vkCreatePipelineLayout(m_vulkan_rhi->m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout)) {
			MAGE_THROW(failed to create pipeline layout which in forward renderpass)
		}

		create_info.layout = m_pipeline_layout;

		create_info.renderPass = p_m_render_pass->m_render_pass;
		create_info.subpass = 0;

		if (VK_SUCCESS != vkCreateGraphicsPipelines(m_vulkan_rhi->m_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &m_pipeline)) {
			MAGE_THROW(failed to create forward render pipeline)
		}
	}

	//TODO:vertex input state 应该是dynamic的
	void ForwardRenderSubpass::draw() {
		VkBuffer current_global_buffer_wait_for_update = p_m_render_pass->m_render_resource->m_global_updated_buffer.m_buffers[m_vulkan_rhi->m_current_frame_index];
		void* map_pointer = p_m_render_pass->m_render_resource->m_global_updated_buffer.m_followed_camera_updated_data_pointers[m_vulkan_rhi->m_current_frame_index];

		//buffer,materials, submeshes
		std::map<GUID32, std::map<GUID64, std::map<GUID32, std::vector<VkRenderModel*>>>> model_batch;
		//batch recognizing
		for (VkRenderModel& model : p_m_render_pass->m_render_scene->m_render_models) {
			auto& buffer_batch = model_batch[model.m_mesh_guid32];
			auto& material_batch = buffer_batch[model.m_material_guid64];
			auto& mesh_batch = material_batch[model.m_model_guid32];
			mesh_batch.emplace_back(&model);
		}
		//DONE

		vkCmdBindPipeline(m_vulkan_rhi->m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
		VkViewport binding_viewport = VulkanInfo::aboutVkViewport();
		binding_viewport.x = 0.f; binding_viewport.y = 0.f;
		binding_viewport.width = m_vulkan_rhi->m_swapchain_extent.width; binding_viewport.height = m_vulkan_rhi->m_swapchain_extent.height;
		binding_viewport.minDepth = 0.f; binding_viewport.maxDepth = 1.f;
		vkCmdSetViewport(m_vulkan_rhi->m_command_buffer, 0, 1, &binding_viewport);
		VkRect2D binding_scissor = VulkanInfo::aboutVkRect2D();
		binding_scissor.offset = { 0,0 };
		binding_scissor.extent = m_vulkan_rhi->m_swapchain_extent;
		vkCmdSetScissor(m_vulkan_rhi->m_command_buffer, 0, 1, &binding_scissor);

		//TODO:camera global perframe data and descriptor set update
		int begin_offset{ 0 };
		GlobalBufferPerFrameData perframe_data{};
		perframe_data.m_camera_view_matrix = glm::lookAt(glm::vec3{ 3.f,3.f,3.f }, { 0.f,0.f,0.f }, { 0.f,0.f,1.f });
		perframe_data.m_camera_perspective_matrix = Matrix4x4::Perspective(60.f, float(m_vulkan_rhi->m_swapchain_extent.width) / m_vulkan_rhi->m_swapchain_extent.height, -0.1f, -50.f);
		//perframe_data.m_camera_perspective_matrix[1][1] *= -1;
		*(reinterpret_cast<GlobalBufferPerFrameData*>(reinterpret_cast<uint8_t*>(map_pointer) + begin_offset)) = perframe_data;
		uint32_t offset = begin_offset;
		//DONE

		begin_offset += sizeof(GlobalBufferPerFrameData);
		begin_offset = Mathf::RoundUp(begin_offset, 64);

		for (auto& [buffer_guid, material_batch] : model_batch) {
			auto& buffer = p_m_render_pass->m_render_resource->m_guid_buffer_map[buffer_guid];
			for (auto& [material_guid, mesh_batch] : material_batch) {
				//VkRenderModel* template_model = mesh_batch.begin()->second.front();
				//bind materials
				vkCmdBindDescriptorSets(m_vulkan_rhi->m_command_buffer, 
					VK_PIPELINE_BIND_POINT_GRAPHICS, 
					m_pipeline_layout, 1, 1, 
					&p_m_render_pass->m_render_resource->m_guid_material_map[material_guid].m_descriptor_set, 
					0, nullptr);
				//DONE

				for (auto& [mesh_guid, same_meshes] : mesh_batch) {
					uint32_t total_drawcall_instances = same_meshes.size();
					VkRenderModel* mark_mesh = same_meshes.front();
					
					//bind vertex and index buffer
					auto get_offset_from = [](VkRenderModel* tmodel, int index)->uint32_t {
						return std::get<1>(tmodel->m_mesh_description.m_mesh_data_offset_infos[index]);
					};
					VkBuffer super_block = buffer.m_bi_data;
					VkBuffer buffers[MAGE_VERTEX_ATTRIBUTES_COUNT] = { VK_NULL_HANDLE };
					std::array<VkDeviceSize, MAGE_VERTEX_ATTRIBUTES_COUNT> offsets = {0};
					int index{ 0 };
					while (index < MAGE_VERTEX_ATTRIBUTES_COUNT) {
						if (get_offset_from(mark_mesh, index) != 0xffffffff) {
							while (index < MAGE_VERTEX_ATTRIBUTES_COUNT) {
								auto offset = get_offset_from(mark_mesh, index);
								if (offset == 0xffffffff) break;
								offsets[index] = offset;
								buffers[index++] = super_block;
							}
						}
						++index;
					}
					vkCmdBindVertexBuffers(m_vulkan_rhi->m_command_buffer, 0, MAGE_VERTEX_ATTRIBUTES_COUNT - 2, buffers, offsets.data());

					auto& index_offset_info = mark_mesh->m_mesh_description.m_mesh_data_offset_infos[6];
					//TODO:VK_INDEX_TYPE_UINT8_EXT
					VkIndexType index_type = (std::get<0>(index_offset_info) * 8) == 32 ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
					vkCmdBindIndexBuffer(m_vulkan_rhi->m_command_buffer, super_block, std::get<1>(index_offset_info), index_type);
					//DONE

					//drawcalls
					int total_drawcall_counts = (total_drawcall_instances + MAGE_PERDRAWCALL_MAX_LIMIT - 1) / MAGE_PERDRAWCALL_MAX_LIMIT;
					for (int i{ 0 }; i < total_drawcall_counts; ++i) {
						int perdrawcall_begin = begin_offset;
						int perdrawcall_frag_begin = begin_offset + sizeof(GlobalBufferPerDrawcallVertexShaderData);
						perdrawcall_frag_begin = Mathf::RoundUp(perdrawcall_frag_begin, 64);

						//populate the instance data
						GlobalBufferPerDrawcallVertexShaderData* transforms =
							reinterpret_cast<GlobalBufferPerDrawcallVertexShaderData*>(reinterpret_cast<uint8_t*>(map_pointer) + perdrawcall_begin);
						GlobalBufferPerDrawcallFragmentShaderData* factors =
							reinterpret_cast<GlobalBufferPerDrawcallFragmentShaderData*>(reinterpret_cast<uint8_t*>(map_pointer) + perdrawcall_frag_begin);
						int current_instance_counts =
							(total_drawcall_instances - MAGE_PERDRAWCALL_MAX_LIMIT * i) < MAGE_PERDRAWCALL_MAX_LIMIT ?
							(total_drawcall_instances - MAGE_PERDRAWCALL_MAX_LIMIT * i) : MAGE_PERDRAWCALL_MAX_LIMIT;
						for (int j{ 0 }; j < current_instance_counts; ++j) {
							transforms->m_mesh_datas[j].m_matrix = 
								same_meshes[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_mesh_description.m_matrix;
							factors->m_frag_datas[j].m_base_color_factor =
								same_meshes[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_mesh_description.m_base_color_factor;
							factors->m_frag_datas[j].m_metallic_factor =
								same_meshes[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_mesh_description.m_metallic_factor;
							factors->m_frag_datas[j].m_roughness_factor =
								same_meshes[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_mesh_description.m_roughness_factor;
						}
						begin_offset = perdrawcall_frag_begin + sizeof(GlobalBufferPerDrawcallFragmentShaderData);
						begin_offset = Mathf::RoundUp(begin_offset, 64);
						//DONE

						uint32_t dynamic_offsets[] = { offset,perdrawcall_begin,perdrawcall_frag_begin };
						vkCmdBindDescriptorSets(m_vulkan_rhi->m_command_buffer,
							VK_PIPELINE_BIND_POINT_GRAPHICS,
							m_pipeline_layout, 0, 1,
							&p_m_render_pass->m_descriptor_sets.sets[m_vulkan_rhi->m_current_frame_index],
							3, dynamic_offsets);

						//draw
						vkCmdDrawIndexed(m_vulkan_rhi->m_command_buffer, std::get<2>(index_offset_info), current_instance_counts, 0, 0, 0);
					}
				}
			}
		}

		vkCmdEndRenderPass(m_vulkan_rhi->m_command_buffer);
	}
}