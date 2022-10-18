#include<engine_core/render_engine/render_scene.h>

namespace Mage {
	auto& RenderScene::getMeshGUIDGenerator() {
		return m_mesh_guid_generator;
	}

	auto& RenderScene::getMaterialGUIDGenerator() {
		return m_material_guid_generator;
	}
}