#pragma once

#include<functional>
#include<string>

#include<iostream>

//#include"engine_core/input/key_family.h"

namespace Mage {
	/// <summary>
	/// character[0-25]: characters
	/// character[26-29]: up/down/left/right
	/// character[30]: space
	/// character[31]: is caps lock
	/// 
	/// number[0-9]: 0-9
	/// number[10]: is num lock
	/// number[11-16]:+-*/[]
	/// </summary>
	struct KeyFamily {
		uint32_t character;
		uint32_t number;
	};

	/// <summary>
	/// buttons[0]:primary button(offen left button)
	/// buttons[1]:secondary buttion(offen right
	/// buttons[3]:middle button
	/// buttons[4-7]:others
	/// </summary>
	struct MouseFamily {
		uint32_t buttons;
		double m_x;
		double m_y;
		double m_delta_x;
		double m_delta_y;
	};

	class WindowSystem;
	enum KeyCode;
	enum InputMode;
	//TODO:input manager key map
	class InputSystem {
	public:
		void initialize(std::shared_ptr<WindowSystem>);

		void tick();

		bool GetMouseButton(int button);
		bool GetMouseButtonDown(int button);
		bool GetMouseButtonUp(int button);
#ifndef NDEBUG
		void print() {
			std::cout << m_frame_mouse.m_x <<" "<< m_frame_mouse.m_y << std::endl;
		}
#endif
		bool GetKeyDown(KeyCode code);

		bool GetInputMode(InputMode required_mod);



	protected:
		void clearMouseOnFlight();
		void clearKeyOnFlight();

		void onKey(int key, int scancode, int action, int mods);
		void onCursorPos(double xpos, double ypos);
		void onMouseButton(int button, int action, int mods);
	private:
		uint32_t m_inputmode;

		KeyFamily m_key;
		MouseFamily m_mouse;

		KeyFamily m_frame_key;//update every tick, others use this
		MouseFamily m_frame_mouse;
	};
}