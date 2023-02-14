#pragma once


#include<ui/window.h>

namespace Mage {
	class Dock : public Window {
	public:
		Dock(const std::string& name) : Window{ name } { configuration.is_dockspace = true; }
		Dock(const std::string& name, const WindowConfig& config) : Window{ name, config } { configuration.is_dockspace = true; }

		void draw() override;
	private:
		//TODO:��������֧��imgui��viewport���ܣ����Դ��ڵ�����Ȩ����dock window�У���ʱ���д��ڿ�����һ��manager����������ｫ����Ҫ�ĳ�raw pointer
		std::vector<std::shared_ptr<Window>> children;
	};
}