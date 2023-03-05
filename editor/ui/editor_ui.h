#pragma once
#include<memory>

#include<ui/windows/window_manager/window_manager.h>

#include"engine_core/function/id_allocator/id_allocator.h"

namespace Mage {
	class Dock;
	class WindowManager;
	class EditorUI {
		using CreateFunc = std::function<std::shared_ptr<Widget>(const std::string&, void*)>;
	public:
		void initialize();

		void drawUI();
		//std::shared_ptr<Widget> creator(const std::string& lable, const std::string& typename, void* instance);
		static std::unordered_map<std::string, CreateFunc> base_widget_creator;
	private:
		void drawFileContentUI();
	private:
		WindowManager window_manager;

		std::shared_ptr<Dock> dock_window;
	};
}