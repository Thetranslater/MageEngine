#pragma once

#include<memory>

namespace Mage {
	class EditorInput;
	class EditorUI;
	class WindowSystem;
	class RenderSystem;
	class WorldManager;
	class MageEngineGlobalContext;
	class MageEditorGlobalContext {
	public:
		std::shared_ptr<EditorInput> m_editor_input;
		std::shared_ptr<EditorUI> m_editor_ui;
		
		std::weak_ptr<WindowSystem> m_window_system;
		std::weak_ptr<RenderSystem> m_render_system;
		std::weak_ptr<WorldManager> m_world_manager;

		void initialize(MageEngineGlobalContext& engine_context);
	};

	extern MageEditorGlobalContext editor_global_context;
}