#pragma once
#include<memory>

namespace Mage {
	class VulkanRHI;
	class WindowSystem;
	struct RenderPassResources;
	class RenderPass;

	class RenderSystem {
	public:
		RenderSystem() = default;
		~RenderSystem() {};

		//TODO:initialize
		void initialize(std::shared_ptr<WindowSystem> window_system);
		void tick();

		std::shared_ptr<VulkanRHI> getVulkanRHI() { return m_vulkan_rhi; }
	private:
		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };
		std::shared_ptr<RenderPassResources> m_render_resources{ nullptr };
		std::shared_ptr<RenderPass> m_render_pass{ nullptr };
	};
}
