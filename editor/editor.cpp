#include"editor.h"
#include"input/editor_input.h"
#include"context/editor_global_context.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/engine.h"
#include"engine_core/input/input_system.h"

namespace Mage {
	void MageEditor::initialize(MageEngine* engine) {
		m_engine = engine;

		editor_global_context.initialize(engine_global_context);
	}

	void MageEditor::shutdown() {
		//TODO:editor shutdown
		m_engine->shutdownEngine();
	}

	void MageEditor::run() {
		while (true) {
			//calculate delta time
			float delta_time = 1.f;
			//input tick
			engine_global_context.m_input_system->tick();
			//editor tick
			editor_global_context.m_editor_input->tick(delta_time);
			//engine tick
			if (m_engine->tick(delta_time)) {
				return;
			}
		}
	}
}