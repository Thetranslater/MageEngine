#pragma once

#define GLFW_INCLUDE_VULKAN
#include<glfw-3.3.7.bin.WIN64/include/GLFW/glfw3.h>

#include<vector>
#include<functional>
#include<array>

#define MAGE_W_LOCK_KEY_MOD GLFW_LOCK_KEY_MODS
#define MAGE_W_RAW_MOUSE_MOTION_MOD GLFW_RAW_MOUSE_MOTION
#define MAGE_W_CURSOR_MOD GLFW_CURSOR
#define MAGE_W_STICKY_KEYS_MOD GLFW_STICKY_KEYS
#define MAGE_W_STICKY_MOUSE_BUTTONS_MOD GLFW_STICKY_MOUSE_BUTTONS

namespace Mage {

	class WindowSystem {
	public:
		WindowSystem() = default;
		~WindowSystem();

		void initialize(int w = 1280, int h = 720, const char* title = "Mage");
		void pollEvents();
		bool shouldClose();
		void setInputMode(int,int);
		GLFWwindow* getWindow();
		std::array<int, 2> getWindowSize();

		using OnResetFunc		= std::function<void()>;
		using OnKeyFunc			= std::function<void(int, int, int, int)>;
		using OnCharFunc		= std::function<void(unsigned int)>;
		using OnCharModsFunc	= std::function<void(int, unsigned int)>;
		using OnMouseButtonFunc = std::function<void(int, int, int)>;
		using OnCursorPosFunc	= std::function<void(double, double)>;
		using OnCursorEnterFunc = std::function<void(int)>;
		using OnScrollFunc		= std::function<void(double, double)>;
		using OnDropFunc		= std::function<void(int, const char**)>;
		using OnWindowSizeFunc	= std::function<void(int, int)>;
		using OnWindowCloseFunc = std::function<void()>;

		void registerOnResetFunc(OnResetFunc func) { m_onResetFunc.push_back(func); }
		void registerOnKeyFunc(OnKeyFunc func) { m_onKeyFunc.push_back(func); }
		void registerOnCharFunc(OnCharFunc func) { m_onCharFunc.push_back(func); }
		void registerOnCharModsFunc(OnCharModsFunc func) { m_onCharModsFunc.push_back(func); }
		void registerOnMouseButtonFunc(OnMouseButtonFunc func) { m_onMouseButtonFunc.push_back(func); }
		void registerOnCursorPosFunc(OnCursorPosFunc func) { m_onCursorPosFunc.push_back(func); }
		void registerOnCursorEnterFunc(OnCursorEnterFunc func) { m_onCursorEnterFunc.push_back(func); }
		void registerOnScrollFunc(OnScrollFunc func) { m_onScrollFunc.push_back(func); }
		void registerOnDropFunc(OnDropFunc func) { m_onDropFunc.push_back(func); }
		void registerOnWindowSizeFunc(OnWindowSizeFunc func) { m_onWindowSizeFunc.push_back(func); }
		void registerOnWindowCloseFunc(OnWindowCloseFunc func) { m_onWindowCloseFunc.push_back(func); }



	protected:
		//TODO:call back funtions
		static void keyInterface(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onKeyCallBacks(key, scancode, action, mods);
			}
		}
		static void charInterface(GLFWwindow* window, unsigned int codepoint)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onCharCallBacks(codepoint);
			}
		}
		static void charModsInterface(GLFWwindow* window, unsigned int codepoint, int mods)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onCharModsCallBacks(codepoint, mods);
			}
		}
		static void mouseButtonInterface(GLFWwindow* window, int button, int action, int mods)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onMouseButtonCallBacks(button, action, mods);
			}
		}
		static void cursorPosInterface(GLFWwindow* window, double xpos, double ypos)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onCursorPosCallBacks(xpos, ypos);
			}
		}
		static void cursorEnterInterface(GLFWwindow* window, int entered)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onCursorEnterCallBacks(entered);
			}
		}
		static void scrollInterface(GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onScrollCallBacks(xoffset, yoffset);
			}
		}
		static void dropInterface(GLFWwindow* window, int count, const char** paths)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->onDropCallBacks(count, paths);
			}
		}
		static void windowSizeInterface(GLFWwindow* window, int width, int height)
		{
			WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
			if (app)
			{
				app->m_width = width;
				app->m_height = height;
				app->onWindowSizeCallBacks(width, height);
			}
		}
		static void windowCloseInterface(GLFWwindow* window) { glfwSetWindowShouldClose(window, true); }

		void onResetCallBacks() {
			for (auto& func : m_onResetFunc) {
				func();
			}
		}
		void onKeyCallBacks(int key, int scancode, int action, int mods) {
			for (auto& func : m_onKeyFunc) {
				func(key, scancode, action, mods);
			}
		}
		void onCharCallBacks(unsigned int codepoint) {
			for (auto& func : m_onCharFunc) {
				func(codepoint);
			}
		}
		void onCharModsCallBacks(unsigned int codepoints, int mods) {
			for (auto& func : m_onCharModsFunc) {
				func(mods, codepoints);
			}
		}
		void onMouseButtonCallBacks(int button, int action, int mods)
		{
			for (auto& func : m_onMouseButtonFunc)
				func(button, action, mods);
		}
		void onCursorPosCallBacks(double xpos, double ypos)
		{
			for (auto& func : m_onCursorPosFunc)
				func(xpos, ypos);
		}
		void onCursorEnterCallBacks(int entered)
		{
			for (auto& func : m_onCursorEnterFunc)
				func(entered);
		}
		void onScrollCallBacks(double xoffset, double yoffset)
		{
			for (auto& func : m_onScrollFunc)
				func(xoffset, yoffset);
		}
		void onDropCallBacks(int count, const char** paths)
		{
			for (auto& func : m_onDropFunc)
				func(count, paths);
		}
		void onWindowSizeCallBacks(int width, int height)
		{
			for (auto& func : m_onWindowSizeFunc)
				func(width, height);
		}

	private:
		GLFWwindow* m_window{ nullptr };
		int m_width{ 0 };
		int m_height{ 0 };

		bool m_is_focus_mode{ false };

		std::vector<OnResetFunc>       m_onResetFunc;
		std::vector<OnKeyFunc>         m_onKeyFunc;
		std::vector<OnCharFunc>        m_onCharFunc;
		std::vector<OnCharModsFunc>    m_onCharModsFunc;
		std::vector<OnMouseButtonFunc> m_onMouseButtonFunc;
		std::vector<OnCursorPosFunc>   m_onCursorPosFunc;
		std::vector<OnCursorEnterFunc> m_onCursorEnterFunc;
		std::vector<OnScrollFunc>      m_onScrollFunc;
		std::vector<OnDropFunc>        m_onDropFunc;
		std::vector<OnWindowSizeFunc>  m_onWindowSizeFunc;
		std::vector<OnWindowCloseFunc> m_onWindowCloseFunc;
	};
}