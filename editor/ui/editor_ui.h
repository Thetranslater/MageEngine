#pragma once

#include"engine_core/function/id_allocator/id_allocator.h"

namespace Mage {
	class Dock;

	class EditorUI {
	public:
		void drawUI();
	private:
		void drawMenuUI();
		void drawHierachyUI();
		void drawFileContentUI();
		void drawInspectorUI();
		void drawDisplayUI();
	private:
		std::shared_ptr<Dock> dock_window;
	};
}