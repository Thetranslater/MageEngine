#pragma once
#include<core/macro.h>

#include<memory>

#include<engine_core/function/global_context/global_context.h>

namespace Mage {
	class VulkanRHI;
	class RenderResource;
	class RenderPass;
	class RenderScene;
	class RenderCamera;
	struct RenderPendingData;
	struct VkRenderModelInfo;
	struct VkRenderMeshDescription;

	//refering: Mesh, Transform, Light, etc
	class RenderSystem {
		struct RenderSystemMeshCTuple {
			TransformComponent* transform;
		};
		struct RenderSystemLightCTuple {
			TransformComponent* transform;
		};
	public:
		RenderSystem() = default;
		~RenderSystem() {};

		void initialize(std::shared_ptr<WindowSystem> window_system);
		void postSetup();
		void tick();

		void initializeUIBackend();

		inline std::shared_ptr<VulkanRHI>			getVulkanRHI() { return m_vulkan_rhi; }
		inline std::shared_ptr<RenderScene>			getRenderScene() { return m_render_scene; }
		inline std::shared_ptr<RenderCamera>		getRenderCamera() { return m_render_camera; }
		inline std::shared_ptr<RenderResource>		getRenderResource() { return m_render_resource; }
		inline std::shared_ptr<RenderPendingData>	getPendingData() { return m_pending_data; }
		inline auto&							getBatchOnFlight() { return m_batch_on_flight; }

	private:
		void tickMeshComponent();
		void tickLightComponent();//TODO
		void tickRenderResource();//负责资源检查和加载
		void tickRenderPendingData(); //负责处理交换数据 

		void imgui_backend_vulkan_init();
		void imgui_backend_uploadfonts();

		std::shared_ptr<RenderPendingData> m_pending_data{ nullptr };
		std::map<ID, std::map<ID, std::map<ID, std::vector<VkRenderMeshDescription*>>>> m_batch_on_flight;

		std::shared_ptr<VulkanRHI> m_vulkan_rhi{ nullptr };
		std::shared_ptr<RenderResource> m_render_resource{ nullptr }; //global?

		//need abstration 
		std::shared_ptr<RenderPass> m_render_pass{ nullptr };
		std::shared_ptr<RenderPass> m_shadow_pass{ nullptr };

		std::shared_ptr<RenderScene> m_render_scene{ nullptr };
		std::shared_ptr<RenderCamera> m_render_camera{ nullptr };

		std::unordered_map<MeshComponent*, RenderSystemMeshCTuple> meshSibling_tuples;
		std::unordered_map<LightComponent*, RenderSystemLightCTuple> lightSibling_tuples;
	};
}
