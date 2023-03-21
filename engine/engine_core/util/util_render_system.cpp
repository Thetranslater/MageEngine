#include"asset/resource_manager/resource_manager.h"

#include"engine_core/render_system/renderer/vulkanRHI.h"
#include"engine_core/util/util_render_system.h"
#include"engine_core/render_system/render_resource.h"
#include"engine_core/render_system/render_pending_data.h"
#include"engine_core/render_system/render_scene.h"
#include"engine_core/render_system/render_camera.h"
#include"engine_core/render_system/render_system.h"
#include"engine_core/render_system/render_model.h"

namespace Mage {
	namespace Util {
		VkRenderModelInfo GetRenderModelInfoFromComponent(ComponentHandle<MeshComponent>& handle) {
			auto render_scene = engine_global_context.m_render_system->getRenderScene();
			auto loader = engine_global_context.m_resource_manager;

			Model mage_model;
			std::string err;
			bool load_res = loader->loadMageModel(handle->Asset().gltf_model_url, &mage_model, &err, nullptr, false);
			//mesh load(fake),将mesh数据填好打包给render scene
			VkRenderModelInfo info = mage_model.getVkRenderModelInfo();
			info.m_go_id = handle->GetGameObject()->getInstanceID();

			handle->SetLoad();
			return info;
		}

		void BuildRenderPrimitiveBatchJobs(RenderSystem* system) {
			auto& batchOnFlight = system->getBatchOnFlight();
			auto  renderScene   = system->getRenderScene();
			auto  renderCamera  = system->getRenderCamera();
			auto  rhi			= system->getVulkanRHI();
			uint8_t* byteStartPointer = reinterpret_cast<uint8_t*>(
				system->getRenderResource()->m_global_updated_buffer.
					m_followed_camera_updated_data_pointers[rhi->getCurrentFrameIndex()]);

			batchOnFlight.clear();
			for (auto& [id, model] : renderScene->m_render_models) {
				for (auto& primitive : model.m_mesh_descriptions) {
					primitive.m_transform = model.m_model_matrix;
					auto& materialBatch   = batchOnFlight[primitive.m_material_index];
					auto& bufferBatch     = materialBatch[primitive.m_meshes_index];
					auto& primitiveBatch  = bufferBatch[primitive.m_submesh_index];
					primitiveBatch.emplace_back(&primitive);
				}
			}

			//[[globalData (roundUp)] [perDrawCallData (roundUp)]...]
			int globalOffset{ 0 };
			{
				GlobalBufferPerFrameData globalPerFrameData{};
				//camera data
				globalPerFrameData.m_camera_perspective_matrix	= glm::mat4(renderCamera->getPerspectiveMatrix());
				globalPerFrameData.m_camera_view_matrix			= glm::mat4(renderCamera->getViewMatrix());
				globalPerFrameData.m_camera_position			= glm::vec3(renderCamera->position());
				//light data
				globalPerFrameData.m_directional_light_num	= std::min(renderScene->m_directional_lights.get().size(), MAGE_DIRECTIONAL_LIGHT_MAX_LIMIT);
				globalPerFrameData.m_point_light_num		= std::min(renderScene->m_point_lights.get().size(), MAGE_POINT_LIGHT_MAX_LIMIT);
				for (int i{ 0 }; i < globalPerFrameData.m_directional_light_num; ++i) {
					globalPerFrameData.m_directional_lights[i].m_color		= renderScene->m_directional_lights.get()[i].m_color;
					globalPerFrameData.m_directional_lights[i].m_direction	= renderScene->m_directional_lights.get()[i].m_direction;
					globalPerFrameData.m_directional_lights[i].m_intensity	= renderScene->m_directional_lights.get()[i].m_intensity;
				}
				for (int i{ 0 }; i < globalPerFrameData.m_point_light_num; ++i) {
					globalPerFrameData.m_point_lights[i].m_color			= renderScene->m_point_lights.get()[i].m_color;
					globalPerFrameData.m_point_lights[i].m_position			= renderScene->m_point_lights.get()[i].m_position;
					globalPerFrameData.m_point_lights[i].m_intensity		= renderScene->m_point_lights.get()[i].m_intensity;
				}

				*reinterpret_cast<GlobalBufferPerFrameData*>(byteStartPointer + globalOffset) = globalPerFrameData;
			}


			int perdrawcallOffset = Mathf::RoundUp(
				globalOffset + sizeof(GlobalBufferPerFrameData), 
				rhi->getDeviceProperties().limits.minStorageBufferOffsetAlignment);
			{
				for (auto& [m_id, materialBatch] : batchOnFlight) {
					for (auto& [b_id, bufferBatch] : materialBatch) {
						for (auto& [s_id, primitiveBatch] : bufferBatch) {
							int instanceCount = primitiveBatch.size();
							int drawcallCount = (instanceCount + MAGE_PERDRAWCALL_MAX_LIMIT - 1) / MAGE_PERDRAWCALL_MAX_LIMIT;
							for (int i{ 0 }; i < drawcallCount; ++i) {

								int thisDrawCallInstanceCount =
									(instanceCount - MAGE_PERDRAWCALL_MAX_LIMIT * i) < MAGE_PERDRAWCALL_MAX_LIMIT ?
									instanceCount - MAGE_PERDRAWCALL_MAX_LIMIT * i : MAGE_PERDRAWCALL_MAX_LIMIT;

								GlobalBufferPerDrawcallData* thisDrawCallPointer =
									reinterpret_cast<GlobalBufferPerDrawcallData*>(byteStartPointer + perdrawcallOffset);

								for (int j{ 0 }; j < thisDrawCallInstanceCount; ++j) {
									thisDrawCallPointer->m_data[j].m_vertex_data.m_model	 = primitiveBatch[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_matrix;
									thisDrawCallPointer->m_data[j].m_vertex_data.m_transform = primitiveBatch[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_transform;

									thisDrawCallPointer->m_data[j].m_fragment_data.m_base_color_factor =
										primitiveBatch[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_base_color_factor;
									thisDrawCallPointer->m_data[j].m_fragment_data.m_metallic_factor =
										primitiveBatch[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_metallic_factor;
									thisDrawCallPointer->m_data[j].m_fragment_data.m_roughness_factor =
										primitiveBatch[MAGE_PERDRAWCALL_MAX_LIMIT * i + j]->m_roughness_factor;
								}

								perdrawcallOffset += sizeof(GlobalBufferPerDrawcallData);
								perdrawcallOffset = Mathf::RoundUp(perdrawcallOffset, rhi->getDeviceProperties().limits.minStorageBufferOffsetAlignment);
							}
						}
					}
				}
			}
		}
	}
}