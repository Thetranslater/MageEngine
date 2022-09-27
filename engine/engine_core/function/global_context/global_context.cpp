#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/window_system.h"
#include"engine_core/render_engine/render_system.h"

namespace Mage {
	MageEngineGlobalContext engine_global_context;

	void MageEngineGlobalContext::startEngine() {

		m_window_system = std::make_shared<WindowSystem>();
		m_window_system->initialize();

		m_render_system = std::make_shared<RenderSystem>();
		m_render_system->initialize(m_window_system);
	}
}