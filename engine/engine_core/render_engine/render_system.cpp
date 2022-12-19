#include"core/macro.h"

#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"
#include"engine_core/render_engine/render_passes/ForwardRenderPass.h"
#include"engine_core/render_engine/render_scene.h"
#include"engine_core/render_engine/render_resource.h"
#include"engine_core/function/global_context/global_context.h"

#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

#include<vector>

namespace Mage {
	void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system) {
		//rhi setup
		m_vulkan_rhi = std::make_shared<VulkanRHI>();
		m_vulkan_rhi->initialize(window_system);
		//render scene setup
		m_render_scene = std::make_shared<RenderScene>();
		m_render_scene->initialize();

		m_render_resource = std::make_shared<RenderResource>();
		m_render_resource->initialize(m_vulkan_rhi.get());

		m_render_pass = std::make_shared<ForwardRenderPass>();
		RenderPassCreateInfo create_info{};
		create_info.render_global_resource = m_render_resource.get();
		create_info.render_scene = m_render_scene.get();
		m_render_pass->initialize(&create_info);
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

		//load job
		while (not m_render_scene->m_p_scene_load_deque->empty()) {
			auto process_job = m_render_scene->m_p_scene_load_deque->getNextProcess();
			VkRenderMeshURI mesh_data_uri{ std::move(process_job.m_mesh_info.m_buffer_uri) };
			std::vector<VkRenderTextureURI> texture_data_uris;
			texture_data_uris.resize(process_job.m_textures_info.m_uris.size());
			for (int i = 0; i < texture_data_uris.size(); ++i) {
				texture_data_uris[i] = std::move(process_job.m_textures_info.m_uris[i]);
			}

			//process mesh
			auto mesh_guid = m_render_scene->getMeshGUIDGenerator().generateGUID(mesh_data_uri);
			if (not m_render_resource->hasMesh(mesh_guid)) {
				Buffer raw_data;
				bool load_ret = resource_manager->loadAsset(mesh_data_uri.m_uri, &raw_data, nullptr, nullptr);
				assert(load_ret);
				RenderResource::IO_Buffer mesh_creation_param;
				mesh_creation_param = std::move(raw_data);
				bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), mesh_guid, mesh_creation_param);
				assert(create_ret);
			}

			//process texture
			std::vector<GUID32> texture_guids(texture_data_uris.size(), std::numeric_limits<GUID32>{}.max());
			for (int i{ 0 }; i < texture_data_uris.size(); ++i) {
				auto texture_guid = m_render_scene->getTextureGUIDGenerator().generateGUID(texture_data_uris[i]);
				if (not m_render_resource->hasTexture(texture_guid)) {
					Texture raw_texture;
					bool load_ret = resource_manager->loadAsset(texture_data_uris[i].m_uri, &raw_texture, nullptr, nullptr);
					assert(load_ret);
					if (texture_data_uris[i].is_srgb) raw_texture.m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_SRGB;
					RenderResource::IO_Texture texture_creation_param;
					texture_creation_param = std::move(raw_texture);
					bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), mesh_guid, texture_creation_param);
					assert(create_ret);
				}
				texture_guids[i] = texture_guid;
			}

			//create render models
			auto part_mesh_generator = m_render_scene->getPartMeshGUIDGenerator();
			for (uint32_t i{ 0 }; i < process_job.m_mesh_info.m_transfer_mesh_descriptions.size();++i) {
				auto part_mesh_guid = part_mesh_generator.generateGUID({ mesh_guid,i });

				VkRenderModel model;
				model.m_mesh_description = std::move(process_job.m_mesh_info.m_transfer_mesh_descriptions[i]);
				model.m_model_guid32 = part_mesh_guid;
				model.m_mesh_guid32 = mesh_guid;
				model.m_texture_guid32s = {
					texture_guids[process_job.m_mesh_info.m_transfer_mesh_descriptions[i].m_base_color_index],
					texture_guids[process_job.m_mesh_info.m_transfer_mesh_descriptions[i].m_normal_index],
					texture_guids[process_job.m_mesh_info.m_transfer_mesh_descriptions[i].m_metallicroughness_index] };

				m_render_scene->m_render_models.emplace_back(model);
			}

		}

		//TODO: free job
		while (not m_render_scene->m_p_scene_delete_deque->empty()) {
			auto process_job = m_render_scene->m_p_scene_delete_deque->getNextProcess();
		}
	}
}