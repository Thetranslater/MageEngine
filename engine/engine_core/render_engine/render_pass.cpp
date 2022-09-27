#include"engine_core/function/global_context/global_context.h"

#include"engine_core/render_engine/render_pass.h"
#include"engine_core/render_engine/render_system.h"

namespace Mage {

	RenderPassResources* RenderPass::m_resources;

	void RenderPass::initialize(const RenderPassCreateInfo* createInfo) {
		m_vulkan_rhi = engine_global_context.m_render_system->getVulkanRHI();
	}

	void RenderPass::updateGlobalRenderResources(RenderPassResources* global_resources) {
		m_resources = global_resources;
	}
}