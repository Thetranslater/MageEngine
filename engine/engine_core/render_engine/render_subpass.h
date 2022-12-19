#pragma once
#include<vulkan/vulkan.h>

#include<vector>
#include<array>
#include<memory>

#include<engine_core/render_engine/render_vulkan_resources.h>

namespace Mage {
	class VulkanRHI;
	class RenderPass;

	struct SubpassCreateInfo {
		std::shared_ptr<VulkanRHI> m_vulkan_rhi;
		RenderPass* m_render_pass;
	};

	/*
	* ��Ϊ��ͨ���ĳ�ʼ������renderpass���õģ� ����subpass��ʹ�õ���Դ���������Դ��ϲ�renderpass�л�ȡ�����Դ�������������������������renderpass�е���Դ��
	* ���Գ�ʼ��˳��subpass��������Դ��ʼ��֮��
	* subpassָ������������һ��renderpass��subpass������pipeline���������õ�descriptorsets��Ӧ����vector<DescriptorSets*>?�����Լ�rhi��
	*/
	class Subpass {
	public:
		virtual void initialize(SubpassCreateInfo*);

		virtual void draw() {};
		virtual void setupPipeline(const std::vector<int>&) {};
	protected:
		RenderPass* p_m_render_pass;
		std::shared_ptr<VulkanRHI> m_vulkan_rhi{nullptr};

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipeline_layout;
	};
}
