#pragma once

#include<ui/windows/window.h>

namespace Mage {
	class Viewer : public Window {
	public:
		//TODO:viewer window construction
		Viewer(const std::string& id) :Window(id) {};
		Viewer(const std::string& id, const WindowConfig& config) :Window(id, config) {};


		void draw() override;
	private:
		static uint32_t id;
	};
}