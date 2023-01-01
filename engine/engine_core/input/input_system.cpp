#include<glfw-3.3.7.bin.WIN64/include/GLFW/glfw3.h>

#include"core/macro.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/render_engine/window_system.h"
#include"engine_core/input/input_system.h"
#include"engine_core/input/key_family.h"


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
	}

	void InputSystem::tick() {
		m_frame_key = m_key;
	}

	void InputSystem::onKey(int key, int scancode, int action, int mods) {
		if (mods & GLFW_MOD_CAPS_LOCK) {
			m_key.character |= MAGE_KEYFAMILY_CHARACTER_CAPS_LOCK;
		}
		else if (mods & GLFW_MOD_NUM_LOCK) {
			m_key.number |= MAGE_KEYFAMILY_NUMBER_NUM_LOCK;
		}
		else {
			m_key.character &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_CHARACTER_CAPS_LOCK;
			m_key.number &= MAGE_KEYFAMILY_FULL ^ MAGE_KEYFAMILY_NUMBER_NUM_LOCK;
		}

		if (action == GLFW_PRESS) {
			if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z) m_key.character |= (1u << (key - GLFW_KEY_A));
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
			default:
				break;
			}
		}
		else if(action == GLFW_RELEASE) {
			if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z) m_key.character &= (MAGE_KEYFAMILY_FULL ^ (1u << (key - GLFW_KEY_A)));
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
			default:
				break;
			}
		}
	}

	void InputSystem::onCursorPos(double xpos, double ypos) {
		m_delta_x = xpos - m_mouse_x;
		m_delta_y = ypos - m_mouse_y;
		m_mouse_x = xpos;
		m_mouse_y = ypos;
	}
}