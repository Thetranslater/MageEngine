#pragma once

#include<memory>

namespace Mage{
	class WindowSystem;
	//control the editor camera
	class EditorInput {
	public:
		void initialize(std::shared_ptr<WindowSystem>);
		void tick(float delta);

	private:
		void OnKey(int key, int scancode, int action, int mods);
		void OnMouseButton(int button, int action, int mods);
		void OnCursorPos(double xpos, double ypos);
		//TODO:other call backs

		//ÿ֡����ʱ�����λ��
		double m_cursor_pos_x;
		double m_cursor_pos_y;

		double m_cursor_delta_x;
		double m_cursor_delta_y;
		double m_cursor_callback_pos_x;
		double m_cursor_callback_pos_y;
	};
}