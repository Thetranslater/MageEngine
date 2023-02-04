#pragma once
#include<memory>

#include"core/math/vector3.h"
#include"core/math/quaternion.h"
#include"core/math/matrix4x4.h"

namespace Mage {
	class VulkanRHI;
	class WindowSystem;
	class RenderResource;
	class RenderPass;
	class RenderScene;
	class RenderCamera;

	struct RenderPendingCamera {
		Vector3 m_pending_position;
		Quaternion m_pending_rotation;

		float m_pending_fov;
		float m_pending_aspect;
		float m_pending_znear;
		float m_pending_zfar;
	};

	struct RenderPendingEditor {
		uint32_t viewport_width;
		uint32_t viewport_height;
		double viewport_x;
		double viewport_y;
	};

	struct RenderPendingData {
		RenderPendingCamera m_camera;
		RenderPendingEditor m_editor;
	};

	class RenderSystem {
	public:
		RenderSystem() = default;
		~RenderSystem() {};

		void initialize(std::shared_ptr<WindowSystem> window_system);
		void tick();
		void preprocess();//渲染前处理，主要负责资源检查和加载
		//TODO:initializeUI-vulkan backend
		void initializeUIBackend();

		inline std::shared_ptr<VulkanRHI> getVulkanRHI() { return m_vulkan_rhi; }
		inline std::shared_ptr<RenderScene> getRenderScene() { return m_render_scene; }
		inline std::shared_ptr<RenderCamera> getRenderCamera() { return m_render_camera; }
		inline std::shared_ptr<RenderResource> getRenderResource() { return m_render_resource; }
		inline std::shared_ptr<RenderPendingData> getPendingData() { return m_pending_data; }

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
