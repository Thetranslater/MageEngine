#include"input/editor_input.h"
#include"context/editor_global_context.h"

#include"core/math/math.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/window_system.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/input/input_system.h"
#include"engine_core/render_engine/render_camera.h"

namespace Mage {
	void EditorInput::initialize(std::shared_ptr<WindowSystem> window_system) {
		window_system->registerOnKeyFunc(std::bind(&EditorInput::OnKey,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4));
		window_system->registerOnMouseButtonFunc(std::bind(&EditorInput::OnMouseButton,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
		window_system->registerOnCursorPosFunc(std::bind(&EditorInput::OnCursorPos,
			this,
			std::placeholders::_1,
			std::placeholders::_2));
	}
	//TODO::update camera rotation
	void EditorInput::tick(float delta) {
		m_cursor_pos_x = m_cursor_callback_pos_x;
		m_cursor_pos_y = m_cursor_callback_pos_y;

		if (engine_global_context.m_input_system->GetMouseButton(1)) {
			float factor = (2.f * Mathf::Tan(editor_global_context.m_render_system.lock()->getRenderCamera()->fov() / 2.f)) / editor_global_context.m_window_system.lock()->getWindowSize()[0];
			float delta_yaw = -Mathf::Atan(factor * m_cursor_delta_x) * Mathf::Rad2Deg;
			float delta_pitch = Mathf::Atan(factor * m_cursor_delta_y) * Mathf::Rad2Deg;
			std::cout << delta_yaw << ',' << delta_pitch << std::endl;

			editor_global_context.m_render_system.lock()->getRenderCamera()->rotate(delta_yaw, delta_pitch);
		}

		m_cursor_delta_x = 0.f;
		m_cursor_delta_y = 0.f;
	}

	//TODO
	void EditorInput::OnKey(int key, int scancode, int action, int mods) {

	}

	void EditorInput::OnCursorPos(double xpos, double ypos) {
		if (m_cursor_pos_x >= 0.f && m_cursor_pos_y >= 0.f) {

		}
		m_cursor_delta_x = xpos - m_cursor_pos_x;
		m_cursor_delta_y = ypos - m_cursor_pos_y;
		m_cursor_callback_pos_x = xpos;
		m_cursor_callback_pos_y = ypos;
	}

	void EditorInput::OnMouseButton(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			engine_global_context.m_window_system->setInputMode(MAGE_W_CURSOR_MOD, GLFW_CURSOR_DISABLED);
		}
		else {
			engine_global_context.m_window_system->setInputMode(MAGE_W_CURSOR_MOD, GLFW_CURSOR_NORMAL);
		}
	}
}