#include<glfw-3.3.7.bin.WIN64/include/GLFW/glfw3.h>

#include"core/macro.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_system/window_system.h"
#include"engine_core/input/input_system.h"
#include"engine_core/input/key_family.h"

#define MOUSE_BUTTONS_MASK 0x000000ffu
#define MOUSE_DOWN_MASK 0x0000ff00u
#define MOUSE_RELEASE_MASK 0x00ff0000u


namespace Mage {
	void InputSystem::initialize(std::shared_ptr<Mage::WindowSystem> window_system) {

		window_system->registerOnKeyFunc(std::bind(&InputSystem::onKey, 
			this, 
			std::placeholders::_1, 
			std::placeholders::_2,
			std::placeholders::_3, 
			std::placeholders::_4));
		window_system->registerOnCursorPosFunc(std::bind(&InputSystem::onCursorPos,
			this,
			std::placeholders::_1,
			std::placeholders::_2));
		window_system->registerOnMouseButtonFunc(std::bind(&InputSystem::onMouseButton,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
	}

	void InputSystem::tick() {
		m_frame_key = m_key;

		uint32_t down_state_buttons = m_frame_mouse.buttons;
		down_state_buttons = ((~down_state_buttons & m_mouse.buttons) << 8) & MOUSE_DOWN_MASK;
		uint32_t release_state_buttongs = m_frame_mouse.buttons;
		release_state_buttongs = ((~(~release_state_buttongs | m_mouse.buttons)) << 16) & MOUSE_RELEASE_MASK;

		clearMouseOnFlight();

		m_frame_mouse.buttons |= down_state_buttons;
		m_frame_mouse.buttons |= release_state_buttongs;
		m_frame_mouse.buttons |= m_mouse.buttons & MOUSE_BUTTONS_MASK;

		m_frame_mouse.m_delta_x = m_mouse.m_delta_x;
		m_frame_mouse.m_delta_y = m_mouse.m_delta_y;
		m_frame_mouse.m_x = m_mouse.m_x;
		m_frame_mouse.m_y = m_mouse.m_y;
	}

	void InputSystem::clearMouseOnFlight() {
		m_frame_mouse.buttons = 0u;
		m_frame_mouse.m_delta_x = 0.f;
		m_frame_mouse.m_delta_y = 0.f;

		//m_frame_mouse.m_x = 0.f;
		//m_frame_mouse.m_y = 0.f;
	}

	void InputSystem::clearKeyOnFlight() {
		m_frame_key.character = 0u;
		m_frame_key.number = 0u;
		m_frame_key.others = 0u;
	}

	void InputSystem::onKey(int key, int scancode, int action, int mods) {
		if (mods & GLFW_MOD_CAPS_LOCK) {
			m_inputmode |= InputMode::CAPS_LOCK;
		}
		else {
			m_inputmode &= MAGE_KEYFAMILY_FULL ^ InputMode::CAPS_LOCK;
		}
		if (mods & GLFW_MOD_NUM_LOCK) {
			m_inputmode |= InputMode::NUM_LOCK;
		}
		else {
			m_inputmode &= MAGE_KEYFAMILY_FULL ^ InputMode::NUM_LOCK;
		}

		//std::cout << std::hex << key << std::endl;

		if (action == GLFW_PRESS) {
			if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z) m_key.character |= (MAGE_KEYFAMILY_CHARACTER_A << (key - GLFW_KEY_A));
			else if (GLFW_KEY_1 <= key && key <= GLFW_KEY_9) m_key.number |= (MAGE_KEYFAMILY_NUMBER_ALPHA1 << (key - GLFW_KEY_1));
			else if (GLFW_KEY_KP_1 <= key && key <= GLFW_KEY_KP_9) m_key.number |= (MAGE_KEYFAMILY_NUMBER_1 << (key - GLFW_KEY_KP_1));
			else if (GLFW_KEY_F1 <= key && key <= GLFW_KEY_F12) m_key.others |= (MAGE_KEYFAMILY_FUNCTION_1 << (key - GLFW_KEY_F1));

			switch (key)
			{
			case GLFW_KEY_UP:
				m_key.character |= MAGE_KEYFAMILY_CHARACTER_UP;
				break;
			case GLFW_KEY_DOWN:
				m_key.character |= MAGE_KEYFAMILY_CHARACTER_DOWN;
				break;
			case GLFW_KEY_LEFT:
				m_key.character |= MAGE_KEYFAMILY_CHARACTER_LEFT;
				break;
			case GLFW_KEY_RIGHT:
				m_key.character |= MAGE_KEYFAMILY_CHARACTER_RIGHT;
				break;
			case GLFW_KEY_SPACE:
				m_key.character |= MAGE_KEYFAMILY_CHARACTER_SPACE;
				break;
			case GLFW_KEY_CAPS_LOCK:
				m_key.character |= MAGE_KEYFAMILY_CHARACTER_CAPS_LOCK;
				break;
			case GLFW_KEY_0:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_ALPHA0;
				break;
			case GLFW_KEY_KP_0:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_0;
				break;
			case GLFW_KEY_NUM_LOCK:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_NUM_LOCK;
				break;
			case GLFW_KEY_KP_ADD:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_PLUS;
				break;
			case GLFW_KEY_KP_SUBTRACT:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_MINUS;
				break;
			case GLFW_KEY_KP_MULTIPLY:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_MULTIPLY;
				break;
			case GLFW_KEY_KP_DIVIDE:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_DIVIDE;
				break;
			case GLFW_KEY_KP_DECIMAL:
				m_key.number |= MAGE_KEYFAMILY_NUMBER_PERIOD;
				break;
			case GLFW_KEY_TAB:
				m_key.others |= MAGE_KEYFAMILY_TAB;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				m_key.others |= MAGE_KEYFAMILY_LEFT_SHIFT;
				break;
			case GLFW_KEY_RIGHT_SHIFT:
				m_key.others |= MAGE_KEYFAMILY_RIGHT_SHIFT;
				break;
			case GLFW_KEY_LEFT_ALT:
				m_key.others |= MAGE_KEYFAMILY_LEFT_ALT;
				break;
			case GLFW_KEY_RIGHT_ALT:
				m_key.others |= MAGE_KEYFAMILY_RIGHT_ALT;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				m_key.others |= MAGE_KEYFAMILY_LEFT_CONTROL;
				break;
			case GLFW_KEY_RIGHT_CONTROL:
				m_key.others |= MAGE_KEYFAMILY_RIGHT_CONTROL;
				break;
			case GLFW_KEY_DELETE:
				m_key.others |= MAGE_KEYFAMILY_DELETE;
				break;
			case GLFW_KEY_BACKSPACE:
				m_key.others |= MAGE_KEYFAMILY_BACKSPACE;
				break;
			default:
				break;
			}
		}
		else if(action == GLFW_RELEASE) {
			if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z) m_key.character &= (MAGE_KEYFAMILY_FULL ^ (MAGE_KEYFAMILY_CHARACTER_A << (key - GLFW_KEY_A)));
			else if (GLFW_KEY_1 <= key && key <= GLFW_KEY_9) m_key.number &= MAGE_KEYFAMILY_FULL ^ (MAGE_KEYFAMILY_NUMBER_ALPHA1 << (key - GLFW_KEY_1));
			else if (GLFW_KEY_KP_1 <= key && key <= GLFW_KEY_KP_9) m_key.number &= MAGE_KEYFAMILY_FULL ^ (MAGE_KEYFAMILY_NUMBER_1 << (key - GLFW_KEY_KP_1));
			else if (GLFW_KEY_F1 <= key && key <= GLFW_KEY_F12) m_key.others &= MAGE_KEYFAMILY_FULL ^ (MAGE_KEYFAMILY_FUNCTION_1 << (key - GLFW_KEY_F1));

			switch (key)
			{
			case GLFW_KEY_UP:
				m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_UP;
				break;
			case GLFW_KEY_DOWN:
				m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_DOWN;
				break;
			case GLFW_KEY_LEFT:
				m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_LEFT;
				break;
			case GLFW_KEY_RIGHT:
				m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_RIGHT;
				break;
			case GLFW_KEY_SPACE:
				m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_SPACE;
				break;
			case GLFW_KEY_CAPS_LOCK :
				m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_CAPS_LOCK;
				break;
			case GLFW_KEY_0:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_ALPHA0;
				break;
			case GLFW_KEY_KP_0:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_0;
				break;
			case GLFW_KEY_NUM_LOCK:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_NUM_LOCK;
				break;
			case GLFW_KEY_KP_ADD:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_PLUS;
				break;
			case GLFW_KEY_KP_SUBTRACT:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_MINUS;
				break;
			case GLFW_KEY_KP_MULTIPLY:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_MULTIPLY;
				break;
			case GLFW_KEY_KP_DIVIDE:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_DIVIDE;
				break;
			case GLFW_KEY_KP_DECIMAL:
				m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_PERIOD;
				break;
			case GLFW_KEY_TAB:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_TAB;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_LEFT_SHIFT;
				break;
			case GLFW_KEY_RIGHT_SHIFT:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_RIGHT_SHIFT;
				break;
			case GLFW_KEY_LEFT_ALT:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_LEFT_ALT;
				break;
			case GLFW_KEY_RIGHT_ALT:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_RIGHT_ALT;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_LEFT_CONTROL;
				break;
			case GLFW_KEY_RIGHT_CONTROL:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_RIGHT_CONTROL;
				break;
			case GLFW_KEY_DELETE:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_DELETE;
				break;
			case GLFW_KEY_BACKSPACE:
				m_key.others &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_BACKSPACE;
				break;
			default:
				break;
			}
		}
	}
	//TODO:确认更新逻辑是否正确
	void InputSystem::onCursorPos(double xpos, double ypos) {
		m_mouse.m_delta_x = xpos - m_frame_mouse.m_x;
		m_mouse.m_delta_y = ypos - m_frame_mouse.m_y;
		m_mouse.m_x = xpos;
		m_mouse.m_y = ypos;
	}

	void InputSystem::onMouseButton(int button, int action, int mods) {
		//std::cout << button << std::endl;
		if (action == GLFW_PRESS) {
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_1:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON01;
				break;
			case GLFW_MOUSE_BUTTON_2:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON02;
				break;
			case GLFW_MOUSE_BUTTON_3:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON03;
				break;
			case GLFW_MOUSE_BUTTON_4:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON04;
				break;
			case GLFW_MOUSE_BUTTON_5:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON05;
				break;
			case GLFW_MOUSE_BUTTON_6:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON06;
				break;
			case GLFW_MOUSE_BUTTON_7:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON07;
				break;
			case GLFW_MOUSE_BUTTON_8:
				m_mouse.buttons |= MAGE_MOUSEFAMILY_BUTTON08;
				break;
			default:
				break;
			}
		}
		else if (action == GLFW_RELEASE) {
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_1:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON01;
				break;
			case GLFW_MOUSE_BUTTON_2:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON02;
				break;
			case GLFW_MOUSE_BUTTON_3:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON03;
				break;
			case GLFW_MOUSE_BUTTON_4:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON04;
				break;
			case GLFW_MOUSE_BUTTON_5:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON05;
				break;
			case GLFW_MOUSE_BUTTON_6:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON06;
				break;
			case GLFW_MOUSE_BUTTON_7:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON07;
				break;
			case GLFW_MOUSE_BUTTON_8:
				m_mouse.buttons &= MAGE_KEYFAMILY_FULL ^ MAGE_MOUSEFAMILY_BUTTON08;
				break;
			default:
				break;
			}
		}
	}

	bool InputSystem::GetKeyDown(KeyCode code) {
		int convert = code;
		if (convert <= 31) {
			return (m_frame_key.character & (1u << convert)) != 0u;
		}
		else if ((convert - 32) <= 31) {
			return (m_frame_key.number & (1u << (convert - 32))) != 0u;
		}
		else if ((convert - 64) <= 31) {
			return (m_frame_key.others & (1u << (convert - 64))) != 0u;
		}
		return false;
	}

	bool InputSystem::GetMouseButton(int button) {
		return (m_frame_mouse.buttons & (MAGE_MOUSEFAMILY_BUTTON01 << button)) != 0u;
	}

	bool InputSystem::GetMouseButtonDown(int button) {
		return ((m_frame_mouse.buttons & MOUSE_DOWN_MASK) & (MAGE_MOUSEFAMILY_BUTTON01 << (button + 8))) != 0u;
	}

	bool InputSystem::GetMouseButtonUp(int button) {
		return ((m_frame_mouse.buttons & MOUSE_RELEASE_MASK) & (MAGE_MOUSEFAMILY_BUTTON01 << (button + 16))) != 0u;
	}
}