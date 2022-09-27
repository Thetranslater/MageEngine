#pragma once
#include<vulkan/vulkan.h>

#include<engine_core/render_engine/render_vulkan_resources.h>

#include<vector>
#include<memory>

namespace Mage {
	class VulkanRHI;
	class Subpass;

	//struct FrameBuffer
	//{
	//	int width;
	//	int height;
	//	int layer;

	//	VkFramebuffer m_frame_buffer;
	//	std::vector<Attachment> m_attachments;
	//};

	struct RenderPassCreateInfo{};

	struct RenderPassResources {
		VkBuffer m_vertex_buffer;
		VkDeviceMemory m_vertex_buffer_memory;

		VkBuffer m_index_buffer;
		VkDeviceMemory m_index_buffer_memory;
	};

	//primary pipeline

	/*
	* ����renderpass�Ļ��࣬����VkRenderPass����rhiָ�롣
	* һ��renderpass�а�������attachment�Թ�subpass���á�һ��descriptor sets�Թ�pipelinesʹ�á�һ��framebuffersƥ��rhi�е�swapchain��С����Ҫÿһ��image
	* ����һ����Ӧ��framebuffer��һ��subpass���ṩ��ͨ����Ⱦ��һ��resources*ָ��ָ��renderpassʹ�õ�ȫ����Դ�����綥�����ݣ��������ݣ�camera�ȣ��Ƿ�Ӧ���Ǿ�̬����˽�У�����
	* renderpassҲ��render system��ӵ�е����������Ⱦ���ߡ�render pass�ĳ�ʼ��������Ԥ������Դ��Ȼ���ʼ����ͨ����Ȼ���ʼ��frame buffer��
	*/
	class RenderPass
	{
	public:
		//TODO:global_resource
		virtual void initialize(const RenderPassCreateInfo* createInfo);

		virtual void draw() {};

		const VkRenderPass& getVkRenderPass() { return m_render_pass; }
		//virtual VkFramebuffer getVkFrameBuffer();
		//virtual DescriptorSets getDescriptorSets();
		void updateGlobalRenderResources(RenderPassResources* global_resources);
	protected:
		VkRenderPass m_render_pass;
		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };	//vulkanRHI

		Attachments m_attachments;
		DescriptorSets m_descriptor_sets;	//read_only input

		std::vector<VkFramebuffer> m_framebuffers;		//create framebuffer for each swapchain image
		std::vector<std::shared_ptr<Subpass>> m_p_subpasses;				//pipelines

		static RenderPassResources* m_resources;
	};
}