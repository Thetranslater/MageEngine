#include<engine_core/render_engine/render_scene.h>

namespace Mage {
	void RenderScene::initialize() {
		m_p_scene_load_deque = std::make_shared<SceneProcessDeque<VkRenderModelInfo>>();
		m_p_scene_delete_deque = std::make_shared<SceneProcessDeque<VkRenderModelInfo>>();
	}

	GUIDGenerator<VkRenderMeshURI>& RenderScene::getMeshGUIDGenerator() {
		return m_mesh_guid_generator;
	}

	GUIDGenerator<VkRenderTextureURI>& RenderScene::getTextureGUIDGenerator() {
		return m_texture_guid_generator;
	}

	GUIDGenerator<VkRenderPartMesh>& RenderScene::getPartMeshGUIDGenerator() {
		return m_part_mesh_guid_generator;
	}

	GUIDGenerator<VkRenderMaterialDescription, GUID64>& RenderScene::getMaterialGUIDGenerator() {
		return m_material_guid_generator;
	}
}