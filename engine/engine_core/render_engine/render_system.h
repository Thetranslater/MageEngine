#pragma once
#include<core/macro.h>

#include<memory>

namespace Mage {
	class VulkanRHI;
	class WindowSystem;
	class RenderResource;
	class RenderPass;
	class RenderScene;
	class RenderCamera;
	struct RenderPendingData;

	class RenderSystem {
	public:
		RenderSystem() = default;
		~RenderSystem() {};

		void initialize(std::shared_ptr<WindowSystem> window_system);
		void postSetup();
		void tick();
		void preprocess();//渲染前处理，主要负责资源检查和加载

		void initializeUIBackend();

		inline std::shared_ptr<VulkanRHI>			getVulkanRHI() { return m_vulkan_rhi; }
		inline std::shared_ptr<RenderScene>			getRenderScene() { return m_render_scene; }
		inline std::shared_ptr<RenderCamera>		getRenderCamera() { return m_render_camera; }
		inline std::shared_ptr<RenderResource>		getRenderResource() { return m_render_resource; }
		inline std::shared_ptr<RenderPendingData>	getPendingData() { return m_pending_data; }

	private:
		void imgui_backend_vulkan_init();
		void imgui_backend_uploadfonts();

		std::shared_ptr<RenderPendingData> m_pending_data{ nullptr };

		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };
		std::shared_ptr<RenderResource> m_render_resource{ nullptr }; //global?
		std::shared_ptr<RenderPass> m_render_pass{ nullptr };
		std::shared_ptr<RenderScene> m_render_scene{ nullptr };
		std::shared_ptr<RenderCamera> m_render_camera{ nullptr };
	};
}
