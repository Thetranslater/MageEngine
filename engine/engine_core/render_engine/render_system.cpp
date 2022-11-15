#include"core/macro.h"

#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"
#include"engine_core/render_engine/render_passes/DevRenderPass.h"
#include"engine_core/render_engine/render_scene.h"
#include"engine_core/render_engine/render_resource.h"
#include"engine_core/function/global_context/global_context.h"

#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

#include<vector>

static Mage::RenderPassResources global_resources;

namespace Mage {
	void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system) {
		//rhi setup
		m_vulkan_rhi = std::make_shared<VulkanRHI>();
		m_vulkan_rhi->initialize(window_system);
		//render scene setup
		m_render_scene = std::make_shared<RenderScene>();
		m_render_scene->initialize();

		m_render_resource = std::make_shared<RenderResource>();
	}

	//TODO
	void RenderSystem::tick() {
		preprocess();

		m_vulkan_rhi->prepareContext();

		m_vulkan_rhi->waitForFences();

		m_vulkan_rhi->prepareVulkanRHIBeforeRender();

		m_render_pass->draw();

		m_vulkan_rhi->prepareVulkanRHIAfterRender();
	}

	void RenderSystem::preprocess() {
		//资源的检查需要从一个工作队列中处理，队列存储这所有当前帧需要加载的mesh数据。该队列存储在渲染场景类中
		std::shared_ptr<ResourceManager> resource_manager = engine_global_context.m_resource_manager;
		assert(resource_manager);

		while (not m_render_scene->m_p_scene_load_deque->empty()) {
			auto process_job = m_render_scene->m_p_scene_load_deque->getNextProcess();
			VkRenderMeshURI mesh_data_uri{ std::move(process_job.m_mesh_info.m_buffer_uri) };
			std::vector<std::string> texture_data_uris;
			texture_data_uris.resize(process_job.m_textures_info.m_uris.size());
			for (int i = 0; i < texture_data_uris.size(); ++i) {
				texture_data_uris[i] = std::move(process_job.m_textures_info.m_uris[i].m_uri);
			}

			//process
			auto mesh_guid = m_render_scene->getMeshGUIDGenerator().generateGUID(mesh_data_uri);
			if (not m_render_resource->hasMesh(mesh_guid)) {
				Buffer raw_data;
				bool load_ret = resource_manager->loadAsset(mesh_data_uri.m_uri, &raw_data, nullptr, nullptr);
				assert(load_ret);
				std::variant<VkRenderBuffer, std::vector<unsigned char>> mesh_creation_param;
				mesh_creation_param = std::move(raw_data.m_data);
				bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), mesh_guid, mesh_creation_param);
				assert(create_ret);
			}
		}
	}
}