#pragma once
#include<memory>

namespace Mage {
	class RenderSystem;
	class WindowSystem;
	class ResourceManager;

	class MageEngineGlobalContext {
	public:
		void startEngine();
		void shutdownEngine();

		std::shared_ptr<RenderSystem> m_render_system;
		std::shared_ptr<WindowSystem> m_window_system;
		std::shared_ptr<ResourceManager> m_resource_manager;
	};

	extern MageEngineGlobalContext engine_global_context;
}
