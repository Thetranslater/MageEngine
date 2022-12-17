#include"engine_core/render_engine/render_subpass.h"
#include"engine_core/render_engine/renderer/vulkanInfo.h"

namespace Mage {
	void Subpass::initialize(SubpassCreateInfo* createInfo) {
		m_vulkan_rhi = createInfo->m_vulkan_rhi;
		p_m_render_pass = createInfo->m_render_pass;
	}
}