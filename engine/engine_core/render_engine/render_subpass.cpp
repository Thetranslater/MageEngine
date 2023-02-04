#include"engine_core/render_engine/render_subpass.h"
#include"engine_core/render_engine/renderer/vulkanInfo.h"

namespace Mage {
	void Subpass::initialize(SubpassCreateInfo* createInfo) {
		m_vulkan_rhi = createInfo->info_vulkan_rhi;
		p_m_render_pass = createInfo->info_render_pass;
	}
}