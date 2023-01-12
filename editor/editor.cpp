#include"editor.h"

#include"engine_core/engine.h"

namespace Mage {
	void MageEditor::initialize(MageEngine* engine) {
		m_engine = engine;
	}

	void MageEditor::shutdown() {
		//TODO:editor shutdown
		m_engine->shutdownEngine();
	}

	void MageEditor::run() {
		while (true) {
			//calculate delta time
			float delta_time = 1.f;
			//editor tick
			
			//engine tick
			if (!m_engine->tick(delta_time)) {
				return;
			}
		}
	}
}