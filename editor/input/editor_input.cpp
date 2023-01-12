#include"input/editor_input.h"
#include"context/editor_global_context.h"

#include"core/math/math.h"

#include"engine_core/render_engine/window_system.h"
#include"engine_core/render_engine/render_system.h"
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
		m_cursor_pos_x += m_cursor_delta_x;
		m_cursor_pos_y += m_cursor_delta_y;

		float factor = (2.f * Mathf::Tan(editor_global_context.m_render_system->getRenderCamera()->fov() / 2.f)) / editor_global_context.m_window_system->getWindowSize()[0];
		float delta_yaw = Mathf::Atan(factor * m_cursor_delta_x);
		float delta_pitch = Mathf::Atan(factor * m_cursor_delta_y);

	}

	//TODO
	void EditorInput::OnKey(int key, int scancode, int action, int mods) {

	}

	void EditorInput::OnCursorPos(double xpos, double ypos) {
		m_cursor_delta_x = xpos - m_cursor_pos_x;
		m_cursor_delta_y = ypos - m_cursor_delta_y;
	}
}