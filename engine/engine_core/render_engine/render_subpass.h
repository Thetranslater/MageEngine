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
	* 因为子通道的初始化是由renderpass调用的， 所有subpass所使用的资源基本都可以从上层renderpass中获取，所以大多出参数给出索引数组来索引renderpass中的资源。
	* 所以初始化顺序subpass可能在资源初始化之后。
	* subpass指出它依附于哪一个renderpass，subpass包含的pipeline，它所引用的descriptorsets（应该是vector<DescriptorSets*>?），以及rhi。
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
