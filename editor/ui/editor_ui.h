#pragma once

#include<memory>

#include<ui/window_manager.h>

#include"engine_core/function/id_allocator/id_allocator.h"

namespace Mage {
	class Dock;
	class WindowManager;
	class EditorUI {
	public:
		void initialize();

		void drawUI();
	private:
		void drawMenuUI();
		void drawHierachyUI();
		void drawFileContentUI();
		void drawInspectorUI();
		void drawDisplayUI();
	private:
		WindowManager window_manager;

		std::shared_ptr<Dock> dock_window;
	};
}