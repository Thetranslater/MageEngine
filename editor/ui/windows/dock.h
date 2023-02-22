#pragma once

#include<ui/windows/window.h>

namespace Mage {
	class Dock : public Window {
	public:
		Dock(const std::string& name) : Window{ name } { configuration.is_dockspace = true; }
		Dock(const std::string& name, const WindowConfig& config) : Window{ name, config } { configuration.is_dockspace = true; }

		void addWindow(std::shared_ptr<Window> child) { children.emplace_back(child.get()); }

		void draw() override;
	private:
		//TODO:将来可能支持imgui的viewport功能，所以窗口的所有权不在dock window中（届时所有窗口可能由一个manager类管理），这里将来需要改成raw pointer
		std::vector<Window*> children;
	};
}