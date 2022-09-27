#pragma once

#define GLFW_INCLUDE_VULKAN
#include<glfw-3.3.7.bin.WIN64/include/GLFW/glfw3.h>

#include<vector>
#include<array>

namespace Mage {

	class WindowSystem {
	public:
		WindowSystem() = default;
		~WindowSystem();

		void initialize(int w = 1280, int h = 720, const char* title = "Mage");
		void pollEvents();
		bool shouldClose();
		GLFWwindow* getWindow();
		std::array<int, 2> getWindowSize();

		//TODO:register

	protected:
		//TODO:call back funtions

	private:
		GLFWwindow* m_window{ nullptr };
		int m_width{ 0 };
		int m_height{ 0 };

		bool m_is_focus_mode{ false };

		//TODO:input call back function
	};
}