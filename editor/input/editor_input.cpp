#include"input/editor_input.h"
#include"context/editor_global_context.h"

#include"core/math/math.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/window_system.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/input/key_family.h"
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
		//camera rotation
		if (engine_global_context.m_input_system->GetMouseButton(1)) {
			std::shared_ptr<RenderCamera> camera = editor_global_context.m_render_system.lock()->getRenderCamera();
			//camera rotation
			float angular_velocity = 90.f / editor_global_context.m_window_system.lock()->getWindowSize()[0];
			float delta_yaw = -m_cursor_delta_x * angular_velocity;
			float delta_pitch = m_cursor_delta_y * angular_velocity;

			camera->rotate(delta_yaw, delta_pitch);

			Vector3 l = camera->left();
			Vector3 f = camera->forward();
			Vector3 u = camera->up();
			//Vector3 cu = Vector3::Cross(f, l);
			//camera movement
			constexpr float camera_speed_detail_mode = 0.0005f;
			Vector3 direction{ Vector3::zero };
			if (engine_global_context.m_input_system->GetKeyDown(KeyCode::A) or engine_global_context.m_input_system->GetKeyDown(KeyCode::LeftArrow)) {
				direction += camera->left();
			}
			if (engine_global_context.m_input_system->GetKeyDown(KeyCode::W) or engine_global_context.m_input_system->GetKeyDown(KeyCode::UpArrow)) {
				direction += camera->forward();
			}
			if (engine_global_context.m_input_system->GetKeyDown(KeyCode::D) or engine_global_context.m_input_system->GetKeyDown(KeyCode::RightArrow)) {
				direction -= camera->left();
			}
			if (engine_global_context.m_input_system->GetKeyDown(KeyCode::S) or engine_global_context.m_input_system->GetKeyDown(KeyCode::DownArrow)) {
				direction -= camera->forward();
			}
			direction.Normalize();
			direction *= camera_speed_detail_mode;
			if (engine_global_context.m_input_system->GetKeyDown(KeyCode::LeftShift) or engine_global_context.m_input_system->GetKeyDown(KeyCode::RightShift)) {
				direction *= 10;
			}
			camera->move(direction);
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