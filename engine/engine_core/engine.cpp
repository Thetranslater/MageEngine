#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/render_resource.h"
#include"engine_core/engine.h"

#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/window_system.h"
#include"engine_core/render_engine/render_scene.h"
#include"engine_core/input/key_family.h"
#include"engine_core/input/input_system.h"
#include"engine_core/scaffold/world_manager/world_manager.h"

#include"core/macro.h"
#include"core/meta/reflection/reflection_register.h"

#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

namespace Mage {
	void MageEngine::startEngine() {
		Reflection::TypeMetaRegister::Register();
		engine_global_context.startEngine();
	}

	void MageEngine::postInitialize() {
		engine_global_context.m_render_system->postSetup();
	}

	void MageEngine::logicalTick(float delta) {
		//engine_global_context.m_input_system->tick();

		engine_global_context.m_world_manager->tick(delta);
	}

	void MageEngine::renderTick(float delta) {
		engine_global_context.m_render_system->tick();
	}

	bool MageEngine::tick(float delta) {
		logicalTick(delta);
		renderTick(delta);

		engine_global_context.m_window_system->pollEvents();
		
		return engine_global_context.m_window_system->shouldClose();
	}

	//TODO:shut engine
	void MageEngine::shutdownEngine() {
		return;
	}
}