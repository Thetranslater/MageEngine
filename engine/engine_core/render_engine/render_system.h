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

	struct RenderPendingData {
		RenderPendingCamera m_camera;
	};

	class RenderSystem {
	public:
		RenderSystem() = default;
		~RenderSystem() {};

		//TODO:initialize
		void initialize(std::shared_ptr<WindowSystem> window_system);
		void tick();
		void preprocess();//��Ⱦǰ������Ҫ������Դ���ͼ���

		std::shared_ptr<VulkanRHI> getVulkanRHI() { return m_vulkan_rhi; }
		std::shared_ptr<RenderScene> getRenderScene() { return m_render_scene; }
		std::shared_ptr<RenderCamera> getRenderCamera() { return m_render_camera; }

		std::shared_ptr<RenderPendingData> getPendingData() { return m_pending_data; }
	private:
		std::shared_ptr<RenderPendingData> m_pending_data{ nullptr };

		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };
		std::shared_ptr<RenderResource> m_render_resource{ nullptr }; //global?
		std::shared_ptr<RenderPass> m_render_pass{ nullptr };
		std::shared_ptr<RenderScene> m_render_scene{ nullptr };
		std::shared_ptr<RenderCamera> m_render_camera{ nullptr };
	};
}
