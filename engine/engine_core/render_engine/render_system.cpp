#include"core/macro.h"

#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"
#include"engine_core/render_engine/render_passes/ForwardRenderPass.h"
#include"engine_core/render_engine/render_scene.h"
#include"engine_core/render_engine/render_resource.h"
#include"engine_core/render_engine/render_camera.h"
#include"engine_core/function/global_context/global_context.h"

#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

#include<vector>

namespace Mage {
	void RenderSystem::initialize(std::shared_ptr<WindowSystem> window_system) {
		m_pending_data = std::make_shared<RenderPendingData>();
		//rhi setup
		m_vulkan_rhi = std::make_shared<VulkanRHI>();
		m_vulkan_rhi->initialize(window_system);
		//render scene setup
		m_render_scene = std::make_shared<RenderScene>();
		m_render_scene->initialize();

		m_render_camera = std::make_shared<RenderCamera>();
		m_render_camera->setPosition(1.f, 0.f, -2.f);
		m_render_camera->setRotation(Quaternion::FromToRotation(Vector3{ 0.f,0.f,1.f }, Vector3{ -1.f,0.f,2.f }));
		m_render_camera->setFov(10.f);
		m_render_camera->setzNear(0.1f);
		m_render_camera->setzFar(10.f);
		m_render_camera->setAspect(float(m_vulkan_rhi->getSwapchainExtent().width) / m_vulkan_rhi->getSwapchainExtent().height);


		m_render_resource = std::make_shared<RenderResource>();
		m_render_resource->initialize(m_vulkan_rhi.get());

		m_render_pass = std::make_shared<ForwardRenderPass>();
		RenderPassCreateInfo create_info{};
		create_info.render_global_resource = m_render_resource.get();
		create_info.render_scene = m_render_scene.get();
		create_info.render_camera = m_render_camera.get();
		m_render_pass->initialize(&create_info);
	}

	//TODO
	void RenderSystem::tick() {
		preprocess();

		m_vulkan_rhi->prepareContext();

		m_vulkan_rhi->waitForFences();

		//m_render_pass->rebindGlobalBuffer();

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

			//process mesh infos
			auto& mesh_info = process_job.m_mesh_info.m_info;
			auto mesh_guid = m_render_scene->getMeshGUIDGenerator().generateGUID(mesh_info);
			if (not m_render_resource->hasMesh(mesh_guid)) {
				Buffer raw_data;
				if (mesh_info.index() == 0 /* uri */) {
					bool load_ret = resource_manager->loadMageBuffer(std::get<0>(mesh_info).m_uri, &raw_data, nullptr, nullptr);
					assert(load_ret);
				}
				else /* embedded data */ {
					raw_data = std::move(std::get<1>(mesh_info).m_raw);
				}
				RenderResource::IO_Buffer iBuffer;
				iBuffer = std::move(raw_data);
				bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), mesh_guid, iBuffer);
			}

			//process texture
			std::vector<GUID32> texture_guids(process_job.m_textures_info.m_infos.size(), std::numeric_limits<GUID32>{}.max());
			for (int i{ 0 }; i < process_job.m_textures_info.m_infos.size(); ++i) {
				auto& info = process_job.m_textures_info.m_infos[i];
				auto texture_guid = m_render_scene->getTextureGUIDGenerator().generateGUID(info.m_detail);
				Texture raw_texture;
				if (not m_render_resource->hasTexture(texture_guid)) {
					Texture raw_texture;
					if (info.m_detail.index() == 0) {
						bool load_ret = resource_manager->loadMageTexture(std::get<0>(info.m_detail).m_uri, &raw_texture, nullptr, nullptr);
						assert(load_ret);
						if (info.is_srgb) raw_texture.m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_SRGB;
					}
					else {
						raw_texture = std::move(std::get<1>(info.m_detail).m_raw);
					}
					RenderResource::IO_Texture iTexture;
					iTexture = std::move(raw_texture);
					bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), texture_guid, iTexture);
					assert(create_ret);
				}
				texture_guids[i] = texture_guid;
			}

			//TODO:process material
			std::vector<GUID64> material_guids(process_job.m_materials_info.m_infos.size());
			for (int i{ 0 }; i < process_job.m_materials_info.m_infos.size(); ++i) {
				auto& material = process_job.m_materials_info.m_infos[i];
				VkRenderMaterialDescription raw_material;
				raw_material.m_base_color_texture_index = texture_guids[material.m_base_color_texture_index];
				raw_material.m_metallic_roughness_texture_index = texture_guids[material.m_metallic_roughness_texture_index];
				raw_material.m_normal_texture_index = texture_guids[material.m_normal_texture_index];

				auto material_guid = m_render_scene->getMaterialGUIDGenerator().generateGUID(raw_material);
				if (not m_render_resource->hasMaterial(material_guid)) {
					RenderResource::IO_Material material_creation_param = std::move(raw_material);
					bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), material_guid, material_creation_param);
					assert(create_ret);
				}
				material_guids[i] = material_guid;
			}


			//create render models
			auto part_mesh_generator = m_render_scene->getPartMeshGUIDGenerator();
			for (uint32_t i{ 0 }; i < process_job.m_mesh_info.m_transfer_mesh_descriptions.size();++i) {
				auto part_mesh_guid = part_mesh_generator.generateGUID({ mesh_guid,i });

				VkRenderModel model;
				model.m_mesh_description = std::move(process_job.m_mesh_info.m_transfer_mesh_descriptions[i]);
				model.m_model_guid32 = part_mesh_guid;
				model.m_mesh_guid32 = mesh_guid;
				model.m_material_guid64 = material_guids[process_job.m_mesh_info.m_transfer_mesh_descriptions[i].m_material_index];

				m_render_scene->m_render_models.emplace_back(model);
			}

		}

		//TODO: free job
		while (not m_render_scene->m_p_scene_delete_deque->empty()) {
			auto process_job = m_render_scene->m_p_scene_delete_deque->getNextProcess();
		}

		//camera component?
		//m_render_camera->setPosition(m_pending_data->m_camera.m_pending_position);
		//m_render_camera->setRotation(m_pending_data->m_camera.m_pending_rotation);
		//m_render_camera->setFov(m_pending_data->m_camera.m_pending_fov);
		//m_render_camera->setAspect(m_pending_data->m_camera.m_pending_aspect);
		//m_render_camera->setzNear(m_pending_data->m_camera.m_pending_znear);
		//m_render_camera->setzFar(m_pending_data->m_camera.m_pending_zfar);
	}
}