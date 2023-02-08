#pragma once

#include<memory>

namespace Mage {
	class EditorInput;
	class EditorUI;
	class WindowSystem;
	class RenderSystem;
	class MageEngineGlobalContext;
	class MageEditorGlobalContext {
	public:
		std::shared_ptr<EditorInput> m_editor_input;
		std::shared_ptr<EditorUI> m_editor_ui;
		
		std::weak_ptr<WindowSystem> m_window_system;
		std::weak_ptr<RenderSystem> m_render_system;

		void initialize(MageEngineGlobalContext& engine_context);
	};

	extern MageEditorGlobalContext editor_global_context;
}