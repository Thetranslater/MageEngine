#include"engine_core/function/global_context/global_context.h"

#include"engine_core/render_engine/render_pass.h"
#include"engine_core/render_engine/render_system.h"

namespace Mage {
	void RenderPass::initialize(const RenderPassCreateInfo* createInfo) {
		m_vulkan_rhi = engine_global_context.m_render_system->getVulkanRHI();
		m_render_system = createInfo->info_render_system;
	}
}