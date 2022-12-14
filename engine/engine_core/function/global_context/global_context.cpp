#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/window_system.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/input/input_system.h"
#include"engine_core/scaffold/world_manager/world_manager.h"

#include"asset/resource_manager/resource_manager.h"

namespace Mage {
	MageEngineGlobalContext engine_global_context;

	void MageEngineGlobalContext::startEngine() {
		m_resource_manager = std::make_shared<ResourceManager>();

		m_world_manager = std::make_shared<WorldManager>();
		m_world_manager->initialize();

		m_window_system = std::make_shared<WindowSystem>();
		m_window_system->initialize();

		m_input_system = std::make_shared<InputSystem>();
		m_input_system->initialize(m_window_system);

		m_render_system = std::make_shared<RenderSystem>();
		m_render_system->initialize(m_window_system);
	}
}