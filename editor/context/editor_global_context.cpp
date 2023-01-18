#include"context/editor_global_context.h"
#include"input/editor_input.h"

#include"engine_core/function/global_context/global_context.h"

namespace Mage {
	MageEditorGlobalContext editor_global_context;

	void MageEditorGlobalContext::initialize(MageEngineGlobalContext& engine_context) {
		m_render_system = engine_context.m_render_system;
		m_window_system = engine_context.m_window_system;

		m_editor_input = std::make_shared<EditorInput>();
		m_editor_input->initialize(engine_context.m_window_system);
	}
}