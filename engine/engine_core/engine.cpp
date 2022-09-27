#include"engine_core/engine.h"
#include"render_engine/render_system.h"
#include"render_engine/window_system.h"

#include"core/macro.h"
#include"core/meta/reflection/reflection_register.h"
#include"engine_core/function/global_context/global_context.h"

namespace Mage {
	void MageEngine::startEngine() {
		Reflection::TypeMetaRegister::Register();
		engine_global_context.startEngine();
	}

	void MageEngine::tick(float delta_time) {
		engine_global_context.m_render_system->tick();
		//after render tick
		engine_global_context.m_window_system->pollEvents();
	}
}