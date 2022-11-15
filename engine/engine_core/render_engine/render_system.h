#pragma once
#include<memory>

namespace Mage {
	class VulkanRHI;
	class WindowSystem;
	class RenderResource;
	class RenderPass;
	class RenderScene;

	class RenderSystem {
	public:
		RenderSystem() = default;
		~RenderSystem() {};

		//TODO:initialize
		void initialize(std::shared_ptr<WindowSystem> window_system);
		void tick();
		void preprocess();//渲染前处理，主要负责资源检查和加载

		std::shared_ptr<VulkanRHI> getVulkanRHI() { return m_vulkan_rhi; }
		std::shared_ptr<RenderScene> getRenderScene() { return m_render_scene; }
	private:
		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };
		std::shared_ptr<RenderResource> m_render_resource{ nullptr }; //global?
		std::shared_ptr<RenderPass> m_render_pass{ nullptr };
		std::shared_ptr<RenderScene> m_render_scene{ nullptr };
	};
}
