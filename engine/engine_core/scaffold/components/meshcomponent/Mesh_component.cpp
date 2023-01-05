#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

#include"engine_core/scaffold/components/meshcomponent/Mesh_component.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/render_scene.h"
#include"engine_core/function/global_context/global_context.h"


namespace Mage{
	void MeshComponent::tick(float delta) {
		if (should_tick_in_editor) {
			if (not is_loaded) {
				auto render_scene = engine_global_context.m_render_system->getRenderScene();
				auto loader = engine_global_context.m_resource_manager;
				
				Model mage_model;
				std::string err;
				bool load_res = loader->loadMageModel(mesh.gltf_model_url, &mage_model, &err, nullptr, false);
				//mesh load(fake),将mesh数据填好打包给render scene
				VkRenderModelInfo render_model = mage_model.getVkRenderModelInfo();
				render_scene->m_p_scene_load_deque->push_back(render_model);
				is_loaded = true;
			}
		}
	}
}