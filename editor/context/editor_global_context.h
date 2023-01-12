#pragma once

#include<memory>

namespace Mage {
	class EditorInput;
	class WindowSystem;
	class RenderSystem;
	class MageEditorGlobalContext {
	public:
		std::shared_ptr<EditorInput> m_editor_input;
		
		WindowSystem* m_window_system;
		RenderSystem* m_render_system;
	};

	extern MageEditorGlobalContext editor_global_context;
}