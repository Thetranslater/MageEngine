#pragma once

namespace Mage {

	class EditorUI {
	public:
		void drawUI();
	private:
		void drawMenuUI();
		void drawHierachyUI();
		void drawFileContentUI();
		void drawInspectorUI();
		void drawDisplayUI();
	};
}