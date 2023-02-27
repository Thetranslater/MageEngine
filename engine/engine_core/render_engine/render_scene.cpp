#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/render_pending_data.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/render_scene.h"

namespace Mage {
	RenderScene::RenderScene() :
		m_directional_lights{ std::ref(engine_global_context.m_render_system->getPendingData()->m_lights.m_directional) },
		m_point_lights{ std::ref(engine_global_context.m_render_system->getPendingData()->m_lights.m_point) } {}

	void RenderScene::initialize() {
		m_p_scene_load_deque = std::make_shared<SceneProcessDeque<VkRenderModelInfo>>();
		m_p_scene_delete_deque = std::make_shared<SceneProcessDeque<VkRenderModelInfo>>();
	}

	GUIDGenerator<std::variant<VkRenderMeshURI, RawMeshData>>& RenderScene::getMeshGUIDGenerator() {
		return m_mesh_guid_generator;
	}

	GUIDGenerator<std::variant<VkRenderImageURI, RawImageData>>& RenderScene::getImageGUIDGenerator() {
		return m_image_guid_generator;
	}

	GUIDGenerator<VkRenderPartMesh>& RenderScene::getPartMeshGUIDGenerator() {
		return m_part_mesh_guid_generator;
	}

	GUIDGenerator<VkRenderMaterialDescription>& RenderScene::getMaterialGUIDGenerator() {
		return m_material_guid_generator;
	}
}