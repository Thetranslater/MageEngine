#pragma once
#include<memory>

#include"core/meta/reflection/reflection.h"

namespace Mage {
	class RenderSystem;
	class WindowSystem;
	class InputSystem;
	class ResourceManager;

	class MageEngineGlobalContext {
	public:
		void startEngine();
		void shutdownEngine();

		std::shared_ptr<InputSystem> m_input_system;
		std::shared_ptr<RenderSystem> m_render_system;
		std::shared_ptr<WindowSystem> m_window_system;
		std::shared_ptr<ResourceManager> m_resource_manager;
	};

	extern MageEngineGlobalContext engine_global_context;
}
