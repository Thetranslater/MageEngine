#include"engine_core/render_system/window_system.h"
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
		glfwSetWindowUserPointer(m_window, this);
		glfwSetKeyCallback(m_window, keyInterface);
		glfwSetCharCallback(m_window, charInterface);
		glfwSetCharModsCallback(m_window, charModsInterface);
		glfwSetMouseButtonCallback(m_window, mouseButtonInterface);
		glfwSetCursorPosCallback(m_window, cursorPosInterface);
		glfwSetCursorEnterCallback(m_window, cursorEnterInterface);
		glfwSetScrollCallback(m_window, scrollInterface);
		glfwSetDropCallback(m_window, dropInterface);
		glfwSetWindowSizeCallback(m_window, windowSizeInterface);
		glfwSetWindowCloseCallback(m_window, windowCloseInterface);

		glfwSetInputMode(m_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowSystem::pollEvents() {
		glfwPollEvents();
	}

	bool WindowSystem::shouldClose() {
		return glfwWindowShouldClose(m_window);
	}

	GLFWwindow* WindowSystem::getGLFWWindow() {
		return m_window;
	}

	std::array<int, 2> WindowSystem::getWindowSize() {
		return std::array<int, 2>{m_width, m_height};
	}

	void WindowSystem::setInputMode(int mode,int value) {
		glfwSetInputMode(m_window, mode, value);
	}
}