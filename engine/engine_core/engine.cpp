#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/render_resource.h"
#include"engine_core/engine.h"

#include"render_engine/render_system.h"
#include"render_engine/window_system.h"
#include"render_engine/render_scene.h"

#include"core/macro.h"
#include"core/meta/reflection/reflection_register.h"

#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

namespace Mage {
	void MageEngine::startEngine() {
		Reflection::TypeMetaRegister::Register();
		engine_global_context.startEngine();
	}

	void MageEngine::logicalTick(float delta_time) {
		Model markov;
		std::string err;
		std::string warn;
		if (!engine_global_context.m_resource_manager->loadAsset("E:\\Download\\makarov_pistol\\scene.gltf", &markov, &err, &warn,false)) {
			MAGE_THROW(failed to load gltf model)
		}

		auto render_scene = engine_global_context.m_render_system->getRenderScene();
		VkRenderModelInfo markov_info = markov.getVkRenderModelInfo();
		render_scene->m_p_scene_load_deque->push_back(markov_info);
	}

	void MageEngine::renderTick(float delta_time) {
		engine_global_context.m_render_system->tick();
	}

	void MageEngine::tick(float delta_time) {
		logicalTick(delta_time);

		renderTick(delta_time);
		//after render tick
		engine_global_context.m_window_system->pollEvents();
	}

	//TODO:shut engine
	void MageEngine::shutdownEngine() {
		return;
	}
}