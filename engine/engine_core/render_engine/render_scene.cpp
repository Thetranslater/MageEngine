#include<engine_core/render_engine/render_scene.h>

namespace Mage {
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

	GUIDGenerator<VkRenderPartMesh, GUID64>& RenderScene::getPartMeshGUIDGenerator() {
		return m_part_mesh_guid_generator;
	}

	GUIDGenerator<VkRenderMaterialDescription, GUID64>& RenderScene::getMaterialGUIDGenerator() {
		return m_material_guid_generator;
	}
}