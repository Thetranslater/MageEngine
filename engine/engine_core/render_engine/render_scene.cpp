#include<engine_core/render_engine/render_scene.h>

namespace Mage {
	void RenderScene::initialize() {
		m_p_scene_load_deque = std::make_shared<SceneProcessDeque<VkRenderModelInfo>>();
		m_p_scene_delete_deque = std::make_shared<SceneProcessDeque<VkRenderModelInfo>>();
	}

	auto& RenderScene::getMeshGUIDGenerator() {
		return m_mesh_guid_generator;
	}

	auto& RenderScene::getTextureGUIDGenerator() {
		return m_texture_guid_generator;
	}
}