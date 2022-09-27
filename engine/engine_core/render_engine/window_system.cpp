#include"engine_core/render_engine/window_system.h"
#include"core/macro.h"

namespace Mage {

	WindowSystem::~WindowSystem() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void WindowSystem::initialize(int w, int h, const char* title) {
		if (!glfwInit()) {
			MAGE_THROW(failed to initialize glfw lib)
		}

		m_width  = w;
		m_height = h;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
		if (m_window == nullptr) {
			glfwTerminate();
			MAGE_THROW(failed to create glfw window)
		}

		//invoke the registers to set up the call back functions

	}

	void WindowSystem::pollEvents() {
		glfwPollEvents();
	}

	bool WindowSystem::shouldClose() {
		return glfwWindowShouldClose(m_window);
	}

	GLFWwindow* WindowSystem::getWindow() {
		return m_window;
	}

	std::array<int, 2> WindowSystem::getWindowSize() {
		return std::array<int, 2>{m_width, m_height};
	}
}