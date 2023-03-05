#pragma once
#include<memory>

#include<engine_core/function/component_pool/component_pool.h>

namespace Mage {
	class Component;
	class RenderSystem;
	class WindowSystem;
	class InputSystem;
	class ResourceManager;
	class WorldManager;
	class TransformComponent;

	class MageEngineGlobalContext {
	public:
		void startEngine();
		void shutdownEngine();

		std::shared_ptr<InputSystem> m_input_system;
		std::shared_ptr<RenderSystem> m_render_system;
		std::shared_ptr<WindowSystem> m_window_system;
		std::shared_ptr<ResourceManager> m_resource_manager;
		std::shared_ptr<WorldManager> m_world_manager;

		std::shared_ptr<ComponentPool<TransformComponent>> m_transform_component_pool;
	};

	extern MageEngineGlobalContext engine_global_context;
}
