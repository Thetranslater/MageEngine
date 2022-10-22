#include"core/macro.h"

#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"
#include"engine_core/render_engine/render_passes/DevRenderPass.h"
#include"engine_core/render_engine/render_scene.h"

#include<vector>

static Mage::RenderPassResources global_resources;

namespace Mage {
	void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system) {
		//rhi setup
		m_vulkan_rhi = std::make_shared<VulkanRHI>();
		m_vulkan_rhi->initialize(window_system);

		m_render_scene = std::make_shared<RenderScene>();
		m_render_scene->initialize();
		
		//resouces setup
		//尚未编写资源类代码，目前显式复制资源
		/*size_t size_of_vertices = sizeof(vertices[0]) * vertices.size();
		VulkanHelper::bufferCreationHelper(m_vulkan_rhi.get(), size_of_vertices, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, global_resources.m_vertex_buffer, global_resources.m_vertex_buffer_memory);

		size_t size_of_indices = sizeof(indices[0]) * indices.size();
		VulkanHelper::bufferCreationHelper(m_vulkan_rhi.get(), size_of_indices, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, global_resources.m_index_buffer, global_resources.m_index_buffer_memory);

		VulkanHelper::moveDataFromVectorToBuffer(m_vulkan_rhi.get(), vertices.begin(),vertices.end(), global_resources.m_vertex_buffer);
		VulkanHelper::moveDataFromVectorToBuffer(m_vulkan_rhi.get(), indices.begin(),indices.end(), global_resources.m_index_buffer);

		RenderPassCreateInfo createInfo{};
		m_render_pass = std::make_shared<DevRenderPass>();
		m_render_pass->initialize(&createInfo);
		m_render_pass->updateGlobalRenderResources(&global_resources);*/
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
		auto gpu_load_que{ m_render_scene->m_p_scene_load_deque };
		while (!gpu_load_que->empty()) {
			auto& load_job = gpu_load_que->getNextProcess();
			//do load job
		}
	}
}