#pragma once
#include<vulkan/vulkan.h>

#include<engine_core/render_system/render_vulkan_resources.h>

#include<vector>
#include<memory>

namespace Mage {
	class VulkanRHI;
	class Subpass;
	class RenderSystem;
	struct GlobalUpdatedBuffer;

	struct RenderPassCreateInfo{
		RenderSystem* info_render_system;
	};

	//primary pipeline

	/*
	* 一个renderpass中包含数个attachment以供subpass引用。一组descriptor sets以供pipelines使用。一组framebuffers匹配rhi中的swapchain大小，需要每一个image
	* 都有一个对应的framebuffer。一组subpass来提供多通道渲染。一个resources*指针指向renderpass使用的全局资源，比如顶点数据，索引数据，camera等（是否应该是静态还是私有？）。
	* renderpass也是render system所拥有的最基础的渲染管线。render pass的初始化会依次预备好资源，然后初始化子通道，然后初始化frame buffer。
	*/
	class RenderPass
	{
	public:
		//TODO:global_resource
		virtual void initialize(const RenderPassCreateInfo* createInfo);

		virtual void draw() {}; //forward for now, mesh resources should passed from here

		const VkRenderPass& getVkRenderPass() { return m_render_pass; }

		VkRenderPass m_render_pass;
		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };	//vulkanRHI

		Attachments m_attachments;
		DescriptorSets m_descriptor_sets;	//read_only input, for global resources

		std::vector<VkFramebuffer> m_framebuffers;		//create framebuffer for each swapchain image
		std::vector<std::shared_ptr<Subpass>> m_p_subpasses;				//pipelines

		RenderSystem* m_render_system{ nullptr };
	};
}