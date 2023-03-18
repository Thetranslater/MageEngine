#include"imgui-docking/imgui.h"
#include"imgui-docking/backends/imgui_impl_vulkan.h"
#include"imgui-docking/backends/imgui_impl_glfw.h"

#include"core/macro.h"
#include"core/hash.h"

#include"engine_core/render_system/render_pending_data.h"
#include"engine_core/render_system/render_system.h"
#include"engine_core/render_system/renderer/vulkanRHI.h"
#include"engine_core/render_system/renderer/vulkanHelper.h"
#include"engine_core/render_system/render_passes/ForwardRenderPass.h"
#include"engine_core/render_system/render_scene.h"
#include"engine_core/render_system/render_resource.h"
#include"engine_core/render_system/render_camera.h"
#include"engine_core/util/util_render_system.h"

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
		m_render_camera->setPosition(0.f, 0.f, 0.f);
		m_render_camera->setRotation(Quaternion::identity);
		m_render_camera->setFov(90.f);
		m_render_camera->setzNear(0.01f);
		m_render_camera->setzFar(100.f);
		m_render_camera->setAspect(float(m_vulkan_rhi->getSwapchainExtent().width) / m_vulkan_rhi->getSwapchainExtent().height);


		m_render_resource = std::make_shared<RenderResource>();
		m_render_resource->initialize(m_vulkan_rhi.get());
	}

	void RenderSystem::postSetup() {
		m_render_pass = std::make_shared<ForwardRenderPass>();
		RenderPassCreateInfo create_info{};
		create_info.info_render_system = this;
		m_render_pass->initialize(&create_info);
	}

	void RenderSystem::initializeUIBackend() {
		imgui_backend_vulkan_init();
		imgui_backend_uploadfonts();
	}


	void RenderSystem::tick() {
		tickMeshComponent();
		tickLightComponent();

		tickRenderResource();
		tickRenderPendingData();

		m_vulkan_rhi->prepareContext();

		m_vulkan_rhi->waitForFences();

		//m_render_pass->rebindGlobalBuffer();

		bool is_window_size_change = m_vulkan_rhi->prepareVulkanRHIBeforeRender(std::bind(&ForwardRenderPass::recreateAfterRHI, static_cast<ForwardRenderPass*>(m_render_pass.get())));
		if (is_window_size_change) return;

		m_render_pass->draw();

		m_vulkan_rhi->prepareVulkanRHIAfterRender();
	}

	void RenderSystem::tickMeshComponent() {
		auto mesh_component_pool = engine_global_context.m_mesh_component_pool;
		for (auto iter{ mesh_component_pool->begin() }; iter != mesh_component_pool->end(); ++iter) {
			if (iter->GetGameObject()->isDirty()) [[unlikely]] {
				meshSibling_tuples[(MeshComponent*)iter].transform = iter->GetGameObject()->GetComponent(TransformComponent);
			}
			if (!iter->IsLoad()) [[unlikely]] {
				VkRenderModelInfo info{ Util::GetRenderModelInfoFromComponent(iter) };
				info.m_transform = meshSibling_tuples[(MeshComponent*)iter].transform->localToWorldMatrix();

				m_render_scene->m_p_scene_load_deque->push_back(info);
			}
			else {
				auto& render_model = m_render_scene->getRenderModel(iter->GetGameObject()->getInstanceID());
				render_model.m_model_matrix = meshSibling_tuples[(MeshComponent*)iter].transform->localToWorldMatrix();
			}
		}
	}

	void RenderSystem::tickLightComponent() {
		auto light_component_pool = engine_global_context.m_light_component_pool;
		for (auto iter{ light_component_pool->begin() }; iter != light_component_pool->end(); ++iter) {
			if (iter->GetGameObject()->isDirty()) [[unlikely]] {
				lightSibling_tuples[iter].transform = iter->GetGameObject()->GetComponent(TransformComponent);
			}
			TransformComponent* sibling_transform = lightSibling_tuples[iter].transform;
			switch (iter->Type()[0]) {
			case 'D':
			{
				DirectionalLight diLi{};
				diLi.m_direction = sibling_transform->Rotation() * Vector3::down;
				diLi.m_color = iter->Color();
				diLi.m_intensity = iter->Intensity();

				m_pending_data->m_lights.m_directional.emplace_back(diLi);
			}
				break;
			case 'P':
			{
				PointLight pLi{};
				pLi.m_position = sibling_transform->Position();
				pLi.m_color = iter->Color();
				pLi.m_intensity = iter->Intensity();

				m_pending_data->m_lights.m_point.emplace_back(pLi);
			}
				break;
			default:
				break;
			}
		}
	}

	void RenderSystem::tickRenderResource() {
		//资源的检查需要从一个工作队列中处理，队列存储这所有当前帧需要加载的mesh数据。该队列存储在渲染场景类中
		std::shared_ptr<ResourceManager> resource_manager = engine_global_context.m_resource_manager;
		assert(resource_manager);

		//load job
		while (not m_render_scene->m_p_scene_load_deque->empty()) {
			auto process_job = m_render_scene->m_p_scene_load_deque->getNextProcess();
			std::vector<ID> mesh_guids(process_job.m_mesh_info.m_infos.size(), invalid_id);
			std::vector<ID> image_guids(process_job.m_images_info.m_infos.size(), invalid_id);
			std::vector<ID> material_guids(process_job.m_materials_info.m_infos.size(), invalid_id);

			//process mesh infos
			auto& mesh_infos = process_job.m_mesh_info.m_infos;
			for (int i{ 0 }; i < mesh_infos.size(); ++i) {
				auto mesh_guid = m_render_scene->getMeshGUIDGenerator().generateGUID(mesh_infos[i]);
				if (not m_render_resource->hasMesh(mesh_guid)) {
					Buffer raw_data;
					if (mesh_infos[i].index() == 0) {
						bool load_ret = resource_manager->loadMageBuffer(std::get<0>(mesh_infos[i]).m_uri, &raw_data, nullptr, nullptr);
						assert(load_ret);
					}
					else {
						raw_data = std::move(std::get<1>(mesh_infos[i]).m_raw);
					}
					RenderResource::IO_Buffer iBuffer;
					iBuffer = std::move(raw_data);
					bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), mesh_guid, iBuffer);
				}
				mesh_guids[i] = mesh_guid;
			}

			//process texture
			for (int i{ 0 }; i < process_job.m_images_info.m_infos.size(); ++i) {
				auto& info = process_job.m_images_info.m_infos[i];
				auto image_guid = m_render_scene->getImageGUIDGenerator().generateGUID(info.m_detail);
				Image raw_texture;
				if (not m_render_resource->hasTexture(image_guid)) {
					Image raw_texture;
					if (info.m_detail.index() == 0) {
						bool load_ret = resource_manager->loadMageImage(std::get<0>(info.m_detail).m_uri, &raw_texture, nullptr, nullptr);
						assert(load_ret);
						if (info.is_srgb) raw_texture.m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_SRGB;
					}
					else {
						raw_texture = std::move(std::get<1>(info.m_detail).m_raw);
					}
					RenderResource::IO_Texture iTexture;
					iTexture = std::move(raw_texture);
					bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), image_guid, iTexture);
					assert(create_ret);
				}
				image_guids[i] = image_guid;
			}

			//TODO:process material
			for (int i{ 0 }; i < process_job.m_materials_info.m_infos.size(); ++i) {
				auto& material = process_job.m_materials_info.m_infos[i];
				material.m_base_color_texture_index = material.m_base_color_texture_index == -1 ? -1 : image_guids[material.m_base_color_texture_index];
				material.m_metallic_roughness_texture_index = material.m_metallic_roughness_texture_index == -1? -1 : image_guids[material.m_metallic_roughness_texture_index];
				material.m_normal_texture_index = material.m_normal_texture_index == -1 ? -1 : image_guids[material.m_normal_texture_index];

				auto material_guid = m_render_scene->getMaterialGUIDGenerator().generateGUID(material);
				if (not m_render_resource->hasMaterial(material_guid)) {
					RenderResource::IO_Material material_creation_param = std::move(material);
					bool create_ret = m_render_resource->getOrCreateRenderResource(m_vulkan_rhi.get(), material_guid, material_creation_param);
					assert(create_ret);
				}
				material_guids[i] = material_guid;
			}


			//create render models
			auto part_mesh_generator = m_render_scene->getPartMeshGUIDGenerator();
			VkRenderModel model(process_job.m_mesh_info.m_transfer_mesh_descriptions.size());
			model.m_model_matrix = process_job.m_transform;
			model.m_bounding_box = process_job.m_bounding_box;
			for (uint32_t i{ 0 }; i < process_job.m_mesh_info.m_transfer_mesh_descriptions.size();++i) {
				auto& primitive_info = process_job.m_mesh_info.m_transfer_mesh_descriptions[i];
				auto mesh_guid_combined = [&mesh_guids](VkRenderMeshDescription& primitive_des)->ID {
					size_t hash{ 0u };
					primitive_des.m_attribute_infos[0].m_buffer_index = mesh_guids[primitive_des.m_attribute_infos[0].m_buffer_index];
					primitive_des.m_attribute_infos[1].m_buffer_index = mesh_guids[primitive_des.m_attribute_infos[1].m_buffer_index];
					primitive_des.m_attribute_infos[2].m_buffer_index = primitive_des.m_attribute_infos[2].m_buffer_index == -1 ? -1 : mesh_guids[primitive_des.m_attribute_infos[2].m_buffer_index];
					primitive_des.m_attribute_infos[3].m_buffer_index = primitive_des.m_attribute_infos[3].m_buffer_index == -1 ? -1 : mesh_guids[primitive_des.m_attribute_infos[3].m_buffer_index];
					primitive_des.m_attribute_infos[4].m_buffer_index = primitive_des.m_attribute_infos[4].m_buffer_index == -1 ? -1 : mesh_guids[primitive_des.m_attribute_infos[4].m_buffer_index];
					primitive_des.m_attribute_infos[5].m_buffer_index = primitive_des.m_attribute_infos[5].m_buffer_index == -1 ? -1 : mesh_guids[primitive_des.m_attribute_infos[5].m_buffer_index];
					primitive_des.m_attribute_infos[6].m_buffer_index = primitive_des.m_attribute_infos[6].m_buffer_index == -1 ? -1 : mesh_guids[primitive_des.m_attribute_infos[6].m_buffer_index];
					hash_combine(hash,
						primitive_des.m_attribute_infos[0].m_buffer_index,
						primitive_des.m_attribute_infos[1].m_buffer_index,
						primitive_des.m_attribute_infos[2].m_buffer_index,
						primitive_des.m_attribute_infos[3].m_buffer_index,
						primitive_des.m_attribute_infos[4].m_buffer_index,
						primitive_des.m_attribute_infos[5].m_buffer_index,
						primitive_des.m_attribute_infos[6].m_buffer_index);
					return hash;
				};
				primitive_info.m_meshes_index = mesh_guid_combined(primitive_info);
				primitive_info.m_submesh_index = part_mesh_generator.generateGUID({ primitive_info.m_meshes_index,i });
				primitive_info.m_material_index = material_guids[process_job.m_mesh_info.m_transfer_mesh_descriptions[i].m_material_index];

				model.m_mesh_descriptions[i] = std::move(process_job.m_mesh_info.m_transfer_mesh_descriptions[i]);
			}
			m_render_scene->m_render_models[process_job.m_go_id] = std::move(model);

		}

		//TODO: free job
		while (not m_render_scene->m_p_scene_delete_deque->empty()) {
			auto process_job = m_render_scene->m_p_scene_delete_deque->getNextProcess();
		}
	}

	void RenderSystem::tickRenderPendingData() {
		//camera
		m_render_camera->setPosition(m_pending_data->m_camera.m_pending_position);
		m_render_camera->setRotation(m_pending_data->m_camera.m_pending_rotation);
		m_render_camera->setFov(m_pending_data->m_camera.m_pending_fov);
		m_render_camera->setAspect(m_pending_data->m_camera.m_pending_aspect);
		m_render_camera->setzNear(m_pending_data->m_camera.m_pending_znear);
		m_render_camera->setzFar(m_pending_data->m_camera.m_pending_zfar);
	}

	void RenderSystem::imgui_backend_vulkan_init() {
		ImGui_ImplGlfw_InitForVulkan(engine_global_context.m_window_system->getGLFWWindow(), true);
		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.Instance = m_vulkan_rhi->getInstance();
		initInfo.PhysicalDevice = m_vulkan_rhi->getPhysicalDevice();
		initInfo.Device = m_vulkan_rhi->getDevice();
		initInfo.QueueFamily = m_vulkan_rhi->getGraphicsQueueFamilyIndex();
		initInfo.Queue = m_vulkan_rhi->getGraphicsQueue();
		initInfo.PipelineCache = VK_NULL_HANDLE;
		initInfo.DescriptorPool = m_vulkan_rhi->getDescriptorPool();
		initInfo.Subpass = 0;
		initInfo.MinImageCount = m_vulkan_rhi->getSwapchainSize();
		initInfo.ImageCount = m_vulkan_rhi->getSwapchainSize();
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		initInfo.Allocator = VK_NULL_HANDLE;
		initInfo.CheckVkResultFn = nullptr;
		ImGui_ImplVulkan_Init(&initInfo, m_render_pass->m_render_pass);
	}

	void RenderSystem::imgui_backend_uploadfonts() {
		VkCommandBuffer imgui_cmd_buffer = m_vulkan_rhi->recordCommandsTemporarily();

		ImGui_ImplVulkan_CreateFontsTexture(imgui_cmd_buffer);

		m_vulkan_rhi->submitCommandsTemporarily(imgui_cmd_buffer);
		vkDeviceWaitIdle(m_vulkan_rhi->getDevice());

		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}
}