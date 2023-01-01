#pragma once

#include<functional>
#include<string>

#include<iostream>

//#include"engine_core/input/key_family.h"

namespace Mage {
	/// <summary>
	/// character[0-25]: characters/[0-9]:numbers
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

	class WindowSystem;
	class InputSystem {
	public:
		void initialize(std::shared_ptr<WindowSystem>);

		void tick();

		void GetButtonDown(const std::string& name);

#ifndef NDEBUG
		void print() {
			std::cout << m_mouse_x << "," << m_mouse_y << std::endl;
		}
#endif

	protected:
		void onKey(int key, int scancode, int action, int mods);
		void onCursorPos(double xpos, double ypos);
	private:
		KeyFamily m_key;
		double m_mouse_x;
		double m_mouse_y;
		double m_delta_x;
		double m_delta_y;

		KeyFamily m_frame_key;//update every tick, others use this
	};
}