#pragma once

#include<unordered_map>
#include<memory>
#include<string>

#include<ui/window.h>
#include<engine_core/scaffold/game_object.h>

#include<engine_core/function/id_allocator/id_allocator.h>

namespace Mage {
	class Window;
	class GameObject;
	struct WindowConfig;
	class WindowManager {
	public:
		template<typename WindowT, typename condition = std::enable_if_t<std::is_base_of_v<Window, WindowT>>>
		std::shared_ptr<WindowT> createWindow(const std::string& name, const WindowConfig& config) {
			std::string id = name + "##" + std::to_string(window_id_allocator.alloc());
			std::shared_ptr<WindowT> window = std::make_shared<WindowT>(id, config);
			windows.insert(std::make_pair(id, window));
			return window;
		}

		void deleteWindow(const std::string& id) {
			windows.erase(id);
		}

	private:
		//TODO：unordered_map使得一个ID对应一个window，如果我们要创建多个相同ID的window实体，该如何表示。
		std::unordered_map<std::string, std::shared_ptr<Window>> windows;
		IDAllocator window_id_allocator;

		GameObject* selected_object;
	};
}