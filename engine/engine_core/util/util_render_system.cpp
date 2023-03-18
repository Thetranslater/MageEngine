#include"asset/resource_manager/resource_manager.h"

#include"engine_core/util/util_render_system.h"
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
	}
}